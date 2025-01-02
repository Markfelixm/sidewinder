#include "Game.hpp"

#include "raymath.h"

Game::Game()
// :  viewState(SCENE), input(Input()), scene(Scene(input))
{
	box = std::make_unique<SpringBox>((Vector2){300.f, 300.f}, (Vector2){1300.f, 300.f}, (Vector2){1300.f, 700.f}, (Vector2){300.f, 700.f}, 1.f, 10.f, 0.1f);
	shape = std::make_unique<Shape>();
	shape->addPointMass((Vector2){200.f, 200.f}, 10.f);
	shape->addPointMass((Vector2){400.f, 200.f}, 10.f);
	shape->addPointMass((Vector2){300.f, 300.f}, 10.f);
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	DrawLine(0, 250, 1600, 250, BLACK);
	// if (viewState == SCENE)
	// 	scene.draw();
	// box->draw();
	shape->draw();
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	// input.update();
	// if (viewState == SCENE)
	// 	scene.update(deltaTime);
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		box->a.position = GetMousePosition();
		box->a.previousPosition = GetMousePosition();
	}
	box->update(deltaTime);

	shape->rotate(10.1f);
	shape->acceleration.x += 100.f;
	shape->update(deltaTime);
}
