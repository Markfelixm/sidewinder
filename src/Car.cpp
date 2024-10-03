#include "Car.hpp"

#include <math.h>
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Car::Car(const Vector2 &initialPosition)
	: position(initialPosition), previousPosition(initialPosition), netForce({0.0f, 0.0f}),
	  rotation(0.0f), throttle(0.0f), steering(0.0f)
{
	texture = LoadTexture("res/car.png");
}

Car::~Car()
{
	UnloadTexture(texture);
}

void Car::draw() const
{

	static const Rectangle sourceRec = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
	Rectangle destRec = {position.x, position.y, (float)texture.width, (float)texture.height};
	static const Vector2 origin = {texture.width / 2.0f, texture.height / 2.0f};
	DrawTexturePro(texture, sourceRec, destRec, origin, rotation * RAD2DEG + 90, WHITE);

	DrawLineV(position, Vector2Add(position, netForce), BLUE); // TODO: rm

	drawDebugPanel({10.0f, 10.0f}); // TODO: rm
}

void Car::update(float dt)
{
	handleInput();

	accumulateForces();
	verletStep(dt);
	satisfyConstraints();
}

void Car::handleInput()
{
	if (IsKeyDown(KEY_UP))
		throttle += 0.1f;
	if (IsKeyDown(KEY_DOWN))
		throttle -= 0.1f;
	throttle = Clamp(throttle, -1.0f, 1.0f);

	if (IsKeyDown(KEY_LEFT))
		steering = -1.0f;
	else if (IsKeyDown(KEY_RIGHT))
		steering = 1.0f;
	else
		steering = 0.0f;
}

void Car::accumulateForces()
{
	throttle *= 0.9f;
	rotation = rotation + steering * PI / 32.0f;

	Vector2 forward = {cosf(rotation), sinf(rotation)};
	float enginePower = 600.0f;
	Vector2 forwardForce = Vector2Scale(forward, throttle * enginePower);

	netForce = Vector2Scale(netForce, 0.9f); // momentum
	netForce = Vector2Add(netForce, forwardForce);
}

void Car::verletStep(float dt)
{
	Vector2 temp = position;

	Vector2 velocity = Vector2Subtract(position, previousPosition);
	velocity = Vector2Scale(velocity, 0.9f); // friction

	Vector2 force = Vector2Scale(netForce, dt * dt);
	position = Vector2Add(position, Vector2Add(velocity, force));

	previousPosition = temp;
}

void Car::satisfyConstraints()
{
	position = Vector2Clamp(position, {0.0f, 0.0f}, {(float)GetScreenWidth(), (float)GetScreenHeight()});
}

// TODO: rm
void Car::drawDebugPanel(Vector2 panelPosition) const
{
	GuiGroupBox({panelPosition.x, panelPosition.y, 200, 150}, "Debug");

	GuiLabel({panelPosition.x + 10, panelPosition.y + 20, 180, 20}, TextFormat("position: (%.2f, %.2f)", position.x, position.y));
	GuiLabel({panelPosition.x + 10, panelPosition.y + 50, 180, 20}, TextFormat("throttle: %.2f", throttle));
	GuiLabel({panelPosition.x + 10, panelPosition.y + 80, 180, 20}, TextFormat("rotation: %.2f", rotation));
}
