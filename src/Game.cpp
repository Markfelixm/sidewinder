#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

#include <vector>

Game::Game() : gravity(0.f)
{
	Color someColor = YELLOW;
	std::vector<PointMass> pointMasses;
	pointMasses.push_back(PointMass({20.f, 20.f}, 10.f));
	pointMasses.push_back(PointMass({900.f, 200.f}, 10.f));
	pointMasses.push_back(PointMass({1100.f, 700.f}, 10.f));
	pointMasses.push_back(PointMass({200.f, 500.f}, 10.f));
	softBody = new SoftBody(pointMasses);
}

Game::~Game()
{
	delete softBody;
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	GuiSlider((Rectangle){10.f + 100, 10.f + 20, 120, 20}, TextFormat("gravity: %.2f", gravity), NULL, &gravity, 0.f, 1000.f);

	softBody->draw(BLUE, 10.f);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	softBody->applyAcceleration({0.f, gravity});
	softBody->update(deltaTime);
	softBody->satisfyConstraints();
}
