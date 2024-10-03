#pragma once

#include "raylib.h"

class Car
{
public:
	Car(const Vector2 &initialPosition);
	~Car();

	void draw() const;
	void update(float dt);

private:
	Car() = delete;

	void handleInput();
	void accumulateForces();
	void verletStep(float dt);
	void satisfyConstraints();

	Texture2D texture;

	Vector2 position;
	Vector2 previousPosition;
	Vector2 netForce;

	float throttle;
	float steering;
	float rotation;

	void drawDebugPanel(Vector2 panelPosition) const; // TODO: rm
};
