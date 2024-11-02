#pragma once

#include "raylib.h"

class PointMass
{
public:
	PointMass(const Vector2 &initialPosition, const float mass);
	~PointMass();

	void setMass(const float newMass);
	const float getMass() const;
	const float getInverseMass() const;
	void setPosition(const Vector2 &newPosition);
	const Vector2 &getPosition() const;
	const Vector2 &getAcceleration() const;

	const Vector2 determineVelocity() const;
	void applyAcceleration(const Vector2 &acceleration);
	virtual void update(const float deltaTime);
	virtual void satisfyConstraints();
	virtual void draw(const Color &color, const float radius) const;

protected:
	Vector2 position;
	Vector2 previousPosition;
	Vector2 netAcceleration;
	float mass;
	float inverseMass;

private:
	PointMass() = delete;
};
