#pragma once

#include "raylib.h"

class Car
{
public:
	Car(const Vector2 &initialPosition);
	~Car();

	void draw() const;
	void update(float dt);

	const Vector2 &getPosition() const;

private:
	Car() = delete;

	Texture2D texture;

	Vector2 position;
	float rotation;
	float speed;
	float acceleration;
	float brakingAcceleration;
	float idleAcceleration;
};
