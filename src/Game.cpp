#include "Game.hpp"

#include "raymath.h"

Game::Game() : topLeft(Particle({100.f, 100.f})),
			   bottomLeft(Particle({200.f, 500.f})),
			   topRight(Particle({500.f, 100.f})),
			   bottomRight(Particle({500.f, 200.f})),
			   left(&topLeft, &bottomLeft),
			   top(&topLeft, &topRight),
			   right(&topRight, &bottomRight),
			   bottom(&bottomLeft, &bottomRight),
			   crossLeft(&topLeft, &bottomRight),
			   crossRight(&topRight, &bottomLeft)
{
	static const Vector2 gravity = {0.f, 100.f};
	left.setAcceleration(gravity);
	top.setAcceleration(gravity);
	right.setAcceleration(gravity);
	bottom.setAcceleration(gravity);
}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	DrawLineEx(topLeft.getPosition(), bottomLeft.getPosition(), 6.f, GREEN);
	DrawLineEx(topLeft.getPosition(), topRight.getPosition(), 6.f, YELLOW);
	DrawLineEx(topRight.getPosition(), bottomRight.getPosition(), 6.f, BLUE);
	DrawLineEx(bottomLeft.getPosition(), bottomRight.getPosition(), 6.f, RED);
	DrawLineEx(topLeft.getPosition(), bottomRight.getPosition(), 6.f, MAROON);
	DrawLineEx(topRight.getPosition(), bottomLeft.getPosition(), 6.f, PINK);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	topLeft.update(deltaTime);
	topRight.update(deltaTime);
	bottomLeft.update(deltaTime);
	bottomRight.update(deltaTime);

	left.satisfyConstraints();
	top.satisfyConstraints();
	right.satisfyConstraints();
	bottom.satisfyConstraints();
	crossLeft.satisfyConstraints();
	crossRight.satisfyConstraints();
}
