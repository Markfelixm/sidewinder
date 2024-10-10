#include "Game.hpp"

#include "raymath.h"
#include <random>

Game::Game()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<float> dist(5.f, 5.f);

	static float density = 4.2f;
	for (int i = 0; i < numBalls; ++i)
	{
		Vector2 position = {
			static_cast<float>(GetRandomValue(1, GetScreenWidth())),
			static_cast<float>(GetRandomValue(1, GetScreenHeight()))};
		float radius = fabs(dist(gen));
		float mass = PI * radius * radius * density;
		balls[i] = new Ball(position, radius, mass);
	}
}

Game::~Game()
{
	for (int i = 0; i < numBalls; ++i)
	{
		delete balls[i];
	}
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	for (int i = 0; i < numBalls; ++i)
	{
		DrawCircleV(balls[i]->getPosition(), balls[i]->getRadius(), GREEN);
	}

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float dt)
{
	static const float pullPower = 424.2f;
	static const float frictionPower = -100.f;

	for (int i = 0; i < numBalls; ++i)
	{
		Vector2 normal = Vector2Normalize(Vector2Subtract(GetMousePosition(), balls[i]->getPosition()));
		Vector2 gravity = Vector2Scale(normal, pullPower);
		Vector2 friction = Vector2Scale(Vector2Normalize(balls[i]->determineVelocity()), frictionPower);
		Vector2 accumulation = Vector2Add(friction, gravity);
		balls[i]->setAcceleration(accumulation);
		balls[i]->update(dt);

		for (int j = i + 1; j < numBalls; ++j)
		{
			if (balls[i]->isCollision(*balls[j]))
			{
				balls[i]->resolveCollision(*balls[j]);
			}
		}

		balls[i]->satisfyConstraints();
	}
}
