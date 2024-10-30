#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

bool isSoftBodyCaptured(SoftBody *softBody);

Game::Game() : gravity(0.f), softBodyStiffness(0.05f), softBodyDamping(0.05f), softBodyColor(RED), dragStrength(100.f)
{
	pointMasses.push_back(PointMass({800.f, 300.f}, 10.f));
	pointMasses.push_back(PointMass({1000.f, 200.f}, 10.f));
	pointMasses.push_back(PointMass({1200.f, 300.f}, 10.f));
	pointMasses.push_back(PointMass({1200.f, 400.f}, 10.f));
	pointMasses.push_back(PointMass({1100.f, 600.f}, 10.f));
	pointMasses.push_back(PointMass({800.f, 800.f}, 10.f));
	pointMasses.push_back(PointMass({500.f, 600.f}, 10.f));
	pointMasses.push_back(PointMass({400.f, 400.f}, 10.f));
	pointMasses.push_back(PointMass({400.f, 300.f}, 10.f));
	pointMasses.push_back(PointMass({600.f, 200.f}, 10.f));
	softBody = new SoftBody(pointMasses);
}

Game::~Game()
{
	delete softBody;
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	softBody->draw(softBodyColor, 15.f);

	GuiSlider((Rectangle){10.f + 100, 10.f + 20, 120, 20}, TextFormat("gravity: %.2f", gravity), NULL, &gravity, -20000.f, 20000.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 40, 120, 20}, TextFormat("stiffness: %.2f", softBodyStiffness), NULL, &softBodyStiffness, 0.f, 2.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 60, 120, 20}, TextFormat("damping: %.2f", softBodyDamping), NULL, &softBodyDamping, 0.f, 1.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 80, 120, 20}, TextFormat("dragging strength: %.2f", dragStrength), NULL, &dragStrength, 1.f, 1000.f);
	GuiTextBox((Rectangle){10.f + 100, 10.f + 100, 120, 20}, (char *)TextFormat("rotation: %.2f", softBody->getRotation()), 50, false);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	if (isSoftBodyCaptured(softBody))
	{
		softBodyColor = ORANGE;
		softBody->moveCenter(GetMousePosition(), dragStrength);
	}
	else
		softBodyColor = RED;

	softBody->setStiffness(softBodyStiffness);
	softBody->setDamping(softBodyDamping);
	softBody->applyAcceleration({0.f, gravity});
	softBody->update(deltaTime);
	softBody->satisfyConstraints();
}

bool isSoftBodyCaptured(SoftBody *softBody)
{
	static bool isCaptured = false;

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		if (softBody->getBoundingBox().contains(GetMousePosition()))
			isCaptured = true;
	}
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		isCaptured = false;
	if (isCaptured)
		return true;
	return false;
}
