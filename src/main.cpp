#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "Game.hpp"

static const int screenWidth = 800;
static const int screenHeight = 450;

void UpdateDrawFrame(Game &game);

int main()
{
	InitWindow(screenWidth, screenHeight, "sidewinder");
	Game game = Game();

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateDrawFrame(game);
	}
#endif

	CloseWindow();
}

void UpdateDrawFrame(Game &game)
{
	game.update(GetFrameTime());
	game.draw();
}
