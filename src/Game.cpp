#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : viewState(SCENE), scene(Scene()) {}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	if (viewState == SCENE)
		scene.draw();

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	if (viewState == SCENE)
		scene.update(deltaTime);
}
