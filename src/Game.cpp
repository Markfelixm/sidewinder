#include "Game.hpp"

Game::Game() : car({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}) {}

Game::~Game() {}

void Game::draw() const
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawFPS(GetScreenWidth() - 80, 10);
	car.draw();
	EndDrawing();
}

void Game::update(float dt)
{
	car.update(dt);
}
