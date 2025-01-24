cmake_minimum_required(VERSION 3.30)
project(sidewinder
	VERSION 0.1
	DESCRIPTION "Sidewinder is a car drifting game made with raylib"
	HOMEPAGE_URL "marmulle.dev"
	LANGUAGES CXX
)
	
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	
# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib
		DOWNLOAD_EXTRACT_TIMESTAMP OFF
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG ${RAYLIB_VERSION}
		GIT_PROGRESS TRUE
	)
	FetchContent_GetProperties(raylib)
	if (NOT raylib_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_MakeAvailable(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
	endif()
endif()

# Project
file(GLOB_RECURSE SOURCES
# src/*.cpp
# inc/*.hpp
inc/Vector.hpp
inc/Matrix.hpp
inc/PointMass.hpp
inc/AABB.hpp
inc/Spring.hpp
inc/FixedSpring.hpp
inc/Viewport.hpp
inc/Shape.hpp
inc/Framed.hpp
inc/Planet.hpp
inc/Shuttle.hpp
inc/Collisions.hpp
inc/Input.hpp
inc/Animated.hpp
src/main.cpp
src/Game.cpp
src/Collisions.cpp
src/Viewport.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Debug flags
# Ensure we are using Debug flags when building in debug mode
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set Debug flags (add -O0 to avoid optimizations in debug mode)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")

# Release flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")


target_include_directories(
	${PROJECT_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
	target_link_libraries(${PROJECT_NAME} "-framework IOKit")
	target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
	target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
