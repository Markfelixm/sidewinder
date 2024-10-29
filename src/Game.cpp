#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : gravity(0.f)
{
	pointMasses.push_back(PointMass({100.f, 100.f}, 10.f));
	pointMasses.push_back(PointMass({500.f, 100.f}, 10.f));
	pointMasses.push_back(PointMass({500.f, 500.f}, 10.f));
	pointMasses.push_back(PointMass({100.f, 300.f}, 10.f));
	softBody = new SoftBody(pointMasses);

	// std::vector<Vector2> shapeVertices;
	// shapeVertices.reserve(pointMasses.size());
	// for (const auto &pointMass : pointMasses)
	// 	shapeVertices.push_back(pointMass.getPosition());
	// shape = new Shape(shapeVertices);

	// point = &(pointMasses.at(1));
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
	GuiTextBox((Rectangle){10.f + 100, 10.f + 40, 120, 20}, (char *)TextFormat("rotation: %.2f", softBody->getRotation()), 50, false);

	softBody->draw(BLUE, 10.f);
	// shape->draw(ORANGE, 10.f);
	// point->draw(RED, 20.f);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	softBody->applyAcceleration({0.f, gravity});
	softBody->update(deltaTime);
	softBody->satisfyConstraints();

	// point->applyAcceleration({0.f, gravity});
	// point->update(deltaTime);
}
