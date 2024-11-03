# Sidewinder

A softbody simulation based on [JellyCar's developer's vlog](https://www.youtube.com/watch?v=3OmkehAJoyo).  
Made in C/C++ with raylib.  
It can be played natively or in the browser.

<!-- TODO: web link
TODO: screenshot -->

## Controls

TBD

## Prerequisites

The following are required to build this project:

1. C/C++ compiler
2. CMake (version 3.30 or later)
3. Git
4. (optional, required for web builds) emscripten

## Dependencies

Graphics utilities are provided by [raylib](https://www.raylib.com/index.html), an open-source C library. It will be downloaded and built by the build script if not found.

## Build and Run

Clone the repository

```bash
git clone https://github.com/Markfelixm/sidewinder.git
cd sidewinder
```

This project comes with a cmake build script.  
It's possible to build this project to run natively or in a browser.

### Native

To build an executable:

```bash
cmake -B build/native
cmake --build build/native
```

Run it with:

```bash
./build/native/sidewinder
```

### Web

The game can also be compiled to run in a browser.
This is achieved by compiling to web assembly and javascript, with an html entrypoint.

To build for the web:

```bash
mkdir -p build/web
cd build/web
emcmake cmake ../.. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXECUTABLE_SUFFIX=".html"
emmake make
```

Then, serve the files and navigate to sidewinder.html  
For example, run a local server with:

```bash
python3 -m http.server
```

Open a browser and visit:

```
localhost:8000/sidewinder.html
```
