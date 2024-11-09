#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "Game.hpp"

static const int screenWidth = 1600;
static const int screenHeight = 1000;

void UpdateDrawFrame();

static Game game = Game();

int main()
{
	InitWindow(screenWidth, screenHeight, "sidewinder");

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateDrawFrame();
	}
#endif

	CloseWindow();
}

void UpdateDrawFrame()
{
	game.update(GetFrameTime());
	game.draw();
}
