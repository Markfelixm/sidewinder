#include "Game.hpp"
#include "Collisions.hpp"
#include "raymath.h"

Game::Game()
// :  viewState(SCENE), input(Input()), scene(Scene(input))
{
	PointMass x = PointMass({200.f, 200.f}, 10.f);
	PointMass y = PointMass({180.f, 220.f}, 10.f);
	PointMass z = PointMass({220.f, 220.f}, 10.f);
	std::vector<PointMass> xyz = {x, y, z};
	attacker = std::make_unique<Framed>(xyz, 200.f);

	PointMass a = PointMass({500.f, 500.f}, 10.f);
	PointMass b = PointMass({600.f, 550.f}, 10.f);
	PointMass c = PointMass({700.f, 500.f}, 10.f);
	PointMass d = PointMass({800.f, 700.f}, 10.f);
	PointMass e = PointMass({400.f, 700.f}, 10.f);
	std::vector<PointMass> points = {a, b, c, d, e};
	defender = std::make_unique<Framed>(points, 200.f);
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	Vector2 mouse = GetMousePosition();
	attacker->draw(RED);
	Color color = BLUE;
	if (collides(attacker->actual, defender->actual))
		color = YELLOW;
	defender->draw(color);
	if (contains(defender->actual.points, mouse))
	{
		Edge nearest = findNearestEdge(defender->actual.points, mouse);
		DrawLineEx(nearest.a.position, nearest.b.position, 15.f, ORANGE);
	}
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	attacker->update(deltaTime);
	defender->update(deltaTime);

	Vector2 mouse = GetMousePosition();
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		Vector2 ac = attacker->frame.getCenter();
		Vector2 ad = {mouse.x - ac.x, mouse.y - ac.y};
		attacker->actual.move(ad);
	}
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		Vector2 dc = defender->frame.getCenter();
		Vector2 dd = {mouse.x - dc.x, mouse.y - dc.y};
		defender->actual.move(dd);
	}

	handleCollision(attacker->actual, defender->actual);
}
