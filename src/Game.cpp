#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : gravity({0.f, 0.f}), gravityStrength(0.f), softBodyStiffness(0.03f), softBodyDamping(0.01f), softBodyColor(RED), collider(PointMass({0.f, 0.f}, 10.f)), colliderMass(10.f)
{
	heartMasses.push_back(PointMass({800.f, 300.f}, 10.f));
	heartMasses.push_back(PointMass({1000.f, 200.f}, 10.f));
	heartMasses.push_back(PointMass({1200.f, 300.f}, 10.f));
	heartMasses.push_back(PointMass({1200.f, 400.f}, 10.f));
	heartMasses.push_back(PointMass({1100.f, 600.f}, 10.f));
	heartMasses.push_back(PointMass({800.f, 800.f}, 10.f));
	heartMasses.push_back(PointMass({500.f, 600.f}, 10.f));
	heartMasses.push_back(PointMass({400.f, 400.f}, 10.f));
	heartMasses.push_back(PointMass({400.f, 300.f}, 10.f));
	heartMasses.push_back(PointMass({600.f, 200.f}, 10.f));
	heart = std::make_unique<SoftBody>(heartMasses);

	boxMasses.push_back(PointMass({600.f, 700.f}, 50.f));
	boxMasses.push_back(PointMass({650.f, 700.f}, 50.f));
	boxMasses.push_back(PointMass({650.f, 800.f}, 50.f));
	boxMasses.push_back(PointMass({600.f, 800.f}, 50.f));
	box = std::make_unique<SoftBody>(boxMasses);
}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	heart->draw(softBodyColor, 10.f);
	box->draw(softBodyColor, 20.f);
	collider.draw(GRAY, 10.f);

	GuiSlider((Rectangle){150.f, 30.f, 120.f, 20.f}, TextFormat("gravityStrength: %.3f", gravityStrength), NULL, &gravityStrength, 0.f, 20000.f);
	GuiSlider((Rectangle){150.f, 50.f, 120.f, 20.f}, TextFormat("stiffness: %.3f", softBodyStiffness), NULL, &softBodyStiffness, 0.f, 2.f);
	GuiSlider((Rectangle){150.f, 70.f, 120.f, 20.f}, TextFormat("damping: %.3f", softBodyDamping), NULL, &softBodyDamping, 0.f, 1.f);
	GuiSlider((Rectangle){150.f, 90.f, 120.f, 20.f}, TextFormat("collider mass: %.3f", colliderMass), NULL, &colliderMass, 1.f, 100.f);
	GuiTextBox((Rectangle){150.f, 110.f, 120.f, 20.f}, (char *)TextFormat("rotation: %.3f", heart->getRotation()), 50, false);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	static Vector2 gravityDirection = {0.f, 1.f};

	// if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	// 	gravityDirection = Vector2Normalize(Vector2Subtract(GetMousePosition(), {GetScreenWidth() / 2.f, GetScreenHeight() / 2.f}));

	gravity = Vector2Scale(gravityDirection, gravityStrength);

	heart->setStiffness(softBodyStiffness);
	heart->setDamping(softBodyDamping);
	// heart->applyAcceleration(gravity);
	heart->update(deltaTime);
	heart->satisfyConstraints();

	box->setStiffness(softBodyStiffness);
	box->setDamping(softBodyDamping);
	box->applyAcceleration(gravity);
	box->update(deltaTime);
	box->satisfyConstraints();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		collider.setPosition(GetMousePosition());
		// set previous position too
		collider.update(deltaTime);
		collider.setPosition(GetMousePosition());
		collider.setMass(colliderMass);
	}
	collider.applyAcceleration(gravity);
	collider.update(deltaTime);
	collider.satisfyConstraints();

	box->handleSoftBodyCollision(*heart);
	heart->handleSoftBodyCollision(*box); // redundant?

	if (heart->contains(collider.getPosition()))
		heart->resolveCollision(collider);
}
