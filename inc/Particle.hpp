#pragma once

#include "raylib.h"

class Particle
{
public:
	Particle(const Vector2 &initialPosition);
	~Particle();

	Vector2 getPosition() const;
	void setPosition(const Vector2 &newPosition);
	Vector2 getAcceleration() const;
	void setAcceleration(const Vector2 &acceleration);
	void applyAcceleration(const Vector2 &acceleration);

	void update(float deltaTime);
	virtual void satisfyConstraints();

	Vector2 determineVelocity() const;

private:
	Particle() = delete;

	Vector2 position;
	Vector2 previousPosition;

	Vector2 netAcceleration;
};
