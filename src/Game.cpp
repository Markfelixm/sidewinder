#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : gravity(0.f), softBodyStiffness(0.05f), softBodyDamping(0.1f)
{
	pointMasses.push_back(PointMass({100.f, 100.f}, 10.f));
	pointMasses.push_back(PointMass({500.f, 100.f}, 10.f));
	pointMasses.push_back(PointMass({500.f, 500.f}, 10.f));
	pointMasses.push_back(PointMass({100.f, 300.f}, 10.f));
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

	softBody->draw(BLUE, 10.f);

	GuiSlider((Rectangle){10.f + 100, 10.f + 20, 120, 20}, TextFormat("gravity: %.2f", gravity), NULL, &gravity, -20000.f, 20000.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 40, 120, 20}, TextFormat("stiffness: %.2f", softBodyStiffness), NULL, &softBodyStiffness, 0.f, 2.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 60, 120, 20}, TextFormat("damping: %.2f", softBodyDamping), NULL, &softBodyDamping, 0.f, 1.f);
	GuiTextBox((Rectangle){10.f + 100, 10.f + 80, 120, 20}, (char *)TextFormat("rotation: %.2f", softBody->getRotation()), 50, false);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	softBody->setStiffness(softBodyStiffness);
	softBody->setDamping(softBodyDamping);
	softBody->applyAcceleration({0.f, gravity});
	softBody->update(deltaTime);
	softBody->satisfyConstraints();
}
