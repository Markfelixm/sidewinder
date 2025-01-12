#pragma once

#include "Vector.hpp"

struct PointMass
{

	V2 position;
	V2 previousPosition;
	V2 acceleration;
	float mass;
	float friction;

	PointMass(const V2 &initialPosition, float mass) : position(initialPosition), previousPosition(initialPosition), acceleration({0.f, 0.f}), mass(mass), friction(0.01f) {}

	V2 getVelocity() { return position - previousPosition; }

	void update(const float deltaTime)
	{
		// Verlet Integration
		// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
		// TODO: (2.f - damping)?
		V2 positionComponent = (position * 2.f) - previousPosition;
		V2 accelerationComponent = acceleration * (deltaTime * deltaTime);
		V2 frictionComponent = (position - previousPosition) * -friction;

		previousPosition = position;
		position = positionComponent + accelerationComponent + frictionComponent;

		acceleration = {0.f, 0.f};
	}
};
