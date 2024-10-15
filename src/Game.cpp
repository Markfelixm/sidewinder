#include "Game.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"

Game::Game() : gravity(500.f), particleRadius(20.f),
			   anchor(Particle({530.f, 500.f})),
			   bob(Particle({530.f, 800.f})),
			   a(Particle({1100.f, 300.f})),
			   b(Particle({1300.f, 500.f})),
			   c(Particle({1100.f, 700.f})),
			   d(Particle({900.f, 500.f})),
			   springStiffness(0.1f), springDamping(0.01f),
			   anchoredSpring(&anchor, &bob, 0.f, springStiffness, springDamping),
			   springAB(&a, &b, 282.842712475f, springStiffness, 0.f),
			   springBC(&b, &c, 282.842712475f, springStiffness, 0.f),
			   springCD(&c, &d, 282.842712475f, springStiffness, 0.f),
			   springAD(&a, &d, 282.842712475f, springStiffness, 0.f),
			   springAC(&a, &c, 400.f, springStiffness, 0.f),
			   springBD(&b, &d, 400.f, springStiffness, 0.f)
{
	bob.applyAcceleration({0.f, gravity});
	a.applyAcceleration({0.f, gravity});
	b.applyAcceleration({0.f, gravity});
	c.applyAcceleration({0.f, gravity});
	d.applyAcceleration({0.f, gravity});
}

Game::~Game() {}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// anchored spring
	DrawLineEx(anchor.getPosition(), bob.getPosition(), 8.f, MAROON);
	DrawCircleV(anchor.getPosition(), 10.f, MAROON);
	DrawCircleV(bob.getPosition(), particleRadius, RED);

	// box
	DrawLineEx(a.getPosition(), b.getPosition(), 4.f, BLUE);
	DrawLineEx(b.getPosition(), c.getPosition(), 4.f, BLUE);
	DrawLineEx(c.getPosition(), d.getPosition(), 4.f, BLUE);
	DrawLineEx(a.getPosition(), d.getPosition(), 4.f, BLUE);
	DrawLineEx(a.getPosition(), c.getPosition(), 4.f, GREEN);
	DrawLineEx(b.getPosition(), d.getPosition(), 4.f, GREEN);
	DrawCircleV(a.getPosition(), particleRadius, RED);

	GuiSlider((Rectangle){10.f + 100, 10.f + 20, 120, 20}, TextFormat("gravity: %.2f", gravity), NULL, &gravity, 0.f, 5000.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 40, 120, 20}, TextFormat("stiffness: %.2f", springStiffness), NULL, &springStiffness, 0.f, 1.f);
	GuiSlider((Rectangle){10.f + 100, 10.f + 60, 120, 20}, TextFormat("damping: %.2f", springDamping), NULL, &springDamping, 0.f, 0.5f);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	// anchored spring
	bob.setAcceleration({0.f, gravity});
	bob.update(deltaTime);

	anchoredSpring.setStiffness(springStiffness);
	anchoredSpring.setDamping(springDamping);
	anchoredSpring.satisfyConstraints();

	anchor.setPosition({530.f, 500.f});

	bob.satisfyConstraints();
	mouseDragBob();

	// box
	a.setAcceleration({0.f, gravity});
	b.setAcceleration({0.f, gravity});
	c.setAcceleration({0.f, gravity});
	d.setAcceleration({0.f, gravity});

	a.update(deltaTime);
	b.update(deltaTime);
	c.update(deltaTime);
	d.update(deltaTime);

	springAB.setStiffness(springStiffness);
	springBC.setStiffness(springStiffness);
	springCD.setStiffness(springStiffness);
	springAD.setStiffness(springStiffness);
	springAC.setStiffness(springStiffness);
	springBD.setStiffness(springStiffness);

	springAB.satisfyConstraints();
	springBC.satisfyConstraints();
	springCD.satisfyConstraints();
	springAD.satisfyConstraints();
	springAC.satisfyConstraints();
	springBD.satisfyConstraints();

	a.satisfyConstraints();
	b.satisfyConstraints();
	c.satisfyConstraints();
	d.satisfyConstraints();

	mouseDragBox();
}

void Game::mouseDragBob()
{
	static bool isCaptured = false;

	if (IsMouseButtonPressed(0))
	{
		if (Vector2Length(Vector2Subtract(GetMousePosition(), bob.getPosition())) < particleRadius)
		{
			isCaptured = true;
		}
	}
	if (IsMouseButtonReleased(0))
		isCaptured = false;
	if (isCaptured)
		bob.setPosition(GetMousePosition());
}

void Game::mouseDragBox()
{
	static bool isCaptured = false;

	if (IsMouseButtonPressed(0))
	{
		if (Vector2Length(Vector2Subtract(GetMousePosition(), a.getPosition())) < particleRadius)
		{
			isCaptured = true;
		}
	}
	if (IsMouseButtonReleased(0))
		isCaptured = false;
	if (isCaptured)
		a.setPosition(GetMousePosition());
}
