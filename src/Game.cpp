#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

bool isSoftBodyCaptured(SoftBody *softBody);

Game::Game() : gravity(0.f), softBodyStiffness(0.03f), softBodyDamping(0.01f), softBodyColor(RED), dragStrength(100.f)
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
	softBody = std::make_unique<SoftBody>(pointMasses);
}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	softBody->draw(softBodyColor, 15.f);

	GuiSlider((Rectangle){150.f, 30.f, 120.f, 20.f}, TextFormat("gravity: %.3f", gravity), NULL, &gravity, -20000.f, 20000.f);
	GuiSlider((Rectangle){150.f, 50.f, 120.f, 20.f}, TextFormat("stiffness: %.3f", softBodyStiffness), NULL, &softBodyStiffness, 0.f, 2.f);
	GuiSlider((Rectangle){150.f, 70.f, 120.f, 20.f}, TextFormat("damping: %.3f", softBodyDamping), NULL, &softBodyDamping, 0.f, 1.f);
	GuiSlider((Rectangle){150.f, 90.f, 120.f, 20.f}, TextFormat("drag strength: %.3f", dragStrength), NULL, &dragStrength, 1.f, 1000.f);
	GuiTextBox((Rectangle){150.f, 110.f, 120.f, 20.f}, (char *)TextFormat("rotation: %.3f", softBody->getRotation()), 50, false);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	if (isSoftBodyCaptured(softBody.get()))
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
		if (softBody->contains(GetMousePosition()))
			isCaptured = true;
	}
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		isCaptured = false;
	return isCaptured;
}
