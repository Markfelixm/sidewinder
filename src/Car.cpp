#include "Car.hpp"

#include <math.h>
#include "raymath.h"

// #define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Car::Car(const Vector2 &initialPosition)
	: position(initialPosition), previousPosition(initialPosition), velocity({0.f, 0.f}),
	  netForce({0.f, 0.f}),
	  driveForce({0.f, 0.f}), enginePower(60.f), throttle(0),
	  steerForce({0.f, 0.f}), turnSpeed(5.f), steering(0),
	  rotation(0.f), mass(1224.f), slipCoef(0.9f), dragCoef(0.5f), frictionCoef(15.f)
{
	invMass = 1.f / mass;
	texture = LoadTexture("res/car.png");
}

Car::~Car()
{
	UnloadTexture(texture);
}

void Car::draw()
{
	// TODO: use static_cast<float> ?
	static const Rectangle sourceRec = {0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	Rectangle destRec = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	static const Vector2 origin = {texture.width / 2.f, texture.height / 2.f};
	DrawTexturePro(texture, sourceRec, destRec, origin, rotation + 90.f, WHITE);

	DrawLineEx(position, Vector2Add(position, netForce), 3.f, GREEN);	 // TODO: rm
	DrawLineEx(position, Vector2Add(position, driveForce), 3.f, BLUE);	 // TODO: rm
	DrawLineEx(position, Vector2Add(position, steerForce), 3.f, YELLOW); // TODO: rm
	drawTuningPanel({10.f, 10.f});
}

void Car::update(float dt)
{
	handleInput();

	velocity = Vector2Subtract(position, previousPosition);

	accumulateForces();
	verletStep(dt);
	satisfyConstraints();
}

void Car::handleInput()
{
	throttle = 0;
	if (IsKeyDown(KEY_UP))
		throttle += 1;
	if (IsKeyDown(KEY_DOWN))
		throttle -= 1;

	steering = 0;
	if (IsKeyDown(KEY_RIGHT))
		steering += 1;
	if (IsKeyDown(KEY_LEFT))
		steering -= 1;
}

void Car::accumulateForces()
{
	rotation += steering * turnSpeed;

	// netForce = Vector2Scale(netForce, 0.9f); // momentum
	netForce = {0.f, 0.f};
	// THINK: instead of netforce reset try to counteract realistically
	// Fdrag = -Cdrag * v * |v|
	// Vector2 dragForce = Vector2Scale(velocity, -dragCoef * Vector2Length(velocity));
	// netForce = Vector2Add(netForce, dragForce);
	// Ffriction = -Cfriction * v
	// Vector2 frictionForce = Vector2Scale(velocity, -frictionCoef);
	// netForce = Vector2Add(netForce, frictionForce);

	// forward unit vector, pointing in rotation direction
	Vector2 forward = {cosf(rotation * DEG2RAD), sinf(rotation * DEG2RAD)};

	// drive in rotation's direction
	driveForce = Vector2Scale(forward, throttle * enginePower);
	//
	// float forwardVelocityComponent = Vector2DotProduct(velocity, forward);
	// driveForce = Vector2Add(driveForce, Vector2Scale(forward, -forwardVelocityComponent));

	// netForce = Vector2Scale(velocity, -1.f);
	// float velMag = Vector2Length(velocity);
	// Vector2 velUnit = Vector2Scale(velocity, 1.f / (velMag + __FLT_EPSILON__));
	// Vector2 frictionForce = Vector2Scale(velUnit, -frictionCoef * velMag);
	// netForce = Vector2Add(netForce, frictionForce);
	netForce = Vector2Add(netForce, Vector2Scale(velocity, -100.f));
	netForce = Vector2Add(netForce, driveForce);

	// right lateral unit vector, pointing 90° clockwise of forward
	// Vector2 rightLateral = {forward.y, forward.x};
	// float lateralVelocityComponent = Vector2DotProduct(velocity, rightLateral);
	// steerForce = Vector2Scale(rightLateral, -lateralVelocityComponent);
	// netForce = Vector2Add(netForce, steerForce);

	// float slidePower = Vector2DotProduct(velocity, {forward.y, forward.x});
	// steerForce = Vector2Scale({forward.y, -forward.x}, slidePower * slipCoef);
	// netForce = Vector2Add(netForce, steerForce);
}

void Car::verletStep(float dt)
{
	Vector2 temp = position;

	// velocity = Vector2Subtract(position, previousPosition);
	// velocity = Vector2Scale(velocity, slipCoef);

	Vector2 force = Vector2Scale(netForce, dt * dt);
	position = Vector2Add(position, Vector2Add(velocity, force));
	position = Vector2Add(position, force);

	previousPosition = temp;
}

void Car::satisfyConstraints()
{
	position = Vector2Clamp(position, {0.f, 0.f}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
}

void Car::drawTuningPanel(Vector2 panelPosition)
{
	GuiGroupBox({panelPosition.x, panelPosition.y, 300, 150}, "Tuning");

	// GuiLabel({panelPosition.x + 10, panelPosition.y + 20, 180, 20}, TextFormat("position: (%.2f, %.2f)", position.x, position.y));
	// GuiLabel({panelPosition.x + 10, panelPosition.y + 40, 180, 20}, TextFormat("rotation: %.2f", rotation));

	GuiSlider((Rectangle){panelPosition.x + 100, panelPosition.y + 20, 120, 20}, TextFormat("enginePower: %.2f", enginePower), NULL, &enginePower, 1.f, 10000.f);
	GuiSlider((Rectangle){panelPosition.x + 100, panelPosition.y + 40, 120, 20}, TextFormat("turnSpeed: %.2f", turnSpeed), NULL, &turnSpeed, 0.f, 10.f);

	GuiSlider((Rectangle){panelPosition.x + 100, panelPosition.y + 60, 120, 20}, TextFormat("dragCoef: %.2f", dragCoef), NULL, &dragCoef, 0.f, 1.f);
	GuiSlider((Rectangle){panelPosition.x + 100, panelPosition.y + 80, 120, 20}, TextFormat("frictionCoef: %.2f", frictionCoef), NULL, &frictionCoef, 0.f, 100.f);
	GuiSlider((Rectangle){panelPosition.x + 100, panelPosition.y + 100, 120, 20}, TextFormat("slipCoef: %.2f", slipCoef), NULL, &slipCoef, 0.f, 1.f);
}
