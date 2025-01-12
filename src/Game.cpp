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
	attacker = std::make_unique<Framed>(xyz, 200.08f);

	PointMass a = PointMass({500.f, 500.f}, 100.f);
	PointMass b = PointMass({600.f, 550.f}, 100.f);
	PointMass c = PointMass({700.f, 500.f}, 100.f);
	PointMass d = PointMass({800.f, 700.f}, 100.f);
	PointMass e = PointMass({400.f, 700.f}, 100.f);
	std::vector<PointMass> points = {a, b, c, d, e};
	defender = std::make_unique<Framed>(points, 1000.03f);
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	Vector2 raymouse = GetMousePosition();
	V2 mouse = V2(raymouse.x, raymouse.y);
	attacker->draw(RED);
	Color color = BLUE;
	if (collides(attacker->actual, defender->actual))
		color = YELLOW;
	defender->draw(color);
	if (contains(defender->actual.points, mouse))
	{
		Edge nearest = findNearestEdge(defender->actual.points, mouse);
		DrawLineEx({nearest.a.position.x, nearest.a.position.y}, {nearest.b.position.x, nearest.b.position.y}, 15.f, ORANGE);
	}
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	attacker->update(deltaTime);
	defender->update(deltaTime);

	Vector2 raymouse = GetMousePosition();
	V2 mouse = V2(raymouse.x, raymouse.y);

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		V2 ac = attacker->frame.getCenter();
		V2 ad = {mouse.x - ac.x, mouse.y - ac.y};
		attacker->actual.move(ad);
	}
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		defender->actual.setPosition(mouse);
		defender->frame.setPosition(mouse);
	}

	// TODO: make stiffness range 0 to 1, maybe relative to mass
	handleCollision(attacker->actual, defender->actual);
}
