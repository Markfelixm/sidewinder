#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() {}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// GuiSlider((Rectangle){10.f + 100, 10.f + 20, 120, 20}, TextFormat("gravity: %.2f", gravity), NULL, &gravity, 0.f, 5000.f);
	// GuiSlider((Rectangle){10.f + 100, 10.f + 40, 120, 20}, TextFormat("stiffness: %.2f", springStiffness), NULL, &springStiffness, 0.f, 1.f);
	// GuiSlider((Rectangle){10.f + 100, 10.f + 60, 120, 20}, TextFormat("damping: %.2f", springDamping), NULL, &springDamping, 0.f, 0.5f);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
}
