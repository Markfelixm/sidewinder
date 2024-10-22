#pragma once

#include "raylib.h"

class Car
{
public:
	Car(const Vector2 &initialPosition);
	~Car();

	void draw();
	void update(float dt);

private:
	Car() = delete;

	void handleInput();
	void accumulateForces();
	void verletStep(float dt);
	void satisfyConstraints();

	void drawTuningPanel(Vector2 panelPosition);

	Texture2D texture;

	Vector2 position;
	Vector2 previousPosition;
	Vector2 velocity;

	Vector2 netForce;
	Vector2 driveForce;
	Vector2 steerForce;

	int throttle;
	float enginePower; // TODO: add lookup table
	int steering;
	float turnSpeed;

	float rotation;
	float mass;
	float invMass;
	float slipCoef;
	float dragCoef;
	float frictionCoef;
};
