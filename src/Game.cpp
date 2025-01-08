#include "Game.hpp"
#include "Collisions.hpp"
#include "raymath.h"

Game::Game()
// :  viewState(SCENE), input(Input()), scene(Scene(input))
{
	PointMass a = PointMass({500.f, 500.f}, 10.f);
	PointMass b = PointMass({600.f, 550.f}, 10.f);
	PointMass c = PointMass({700.f, 500.f}, 10.f);
	PointMass d = PointMass({800.f, 700.f}, 10.f);
	PointMass e = PointMass({400.f, 700.f}, 10.f);
	std::vector<PointMass> points = {a, b, c, d, e};
	framed = std::make_unique<Framed>(points, 200.f);
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	Color color = BLUE;
	Vector2 mouse = GetMousePosition();
	if (contains(mouse, framed->actual.points))
		color = YELLOW;
	framed->draw(color);
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{

		framed->actual.rotate(1.5f);
		// framed->frame.rotate(1.5f);
		// framed->actual.points.at(0).position = GetMousePosition();
		// framed->frame.points.at(0).position = GetMousePosition();
	}
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		framed->actual.points.at(3).position = GetMousePosition();
		// framed->actual.rotate(-1.5f);
	}

	framed->update(deltaTime);
}
