#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : viewState(SCENE), input(Input()), scene(Scene(input)) {}

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
	input.update();
	if (viewState == SCENE)
		scene.update(deltaTime);
}
