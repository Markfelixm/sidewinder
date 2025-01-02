#pragma once

#include "raylib.h"
#include "raymath.h"

struct PointMass
{

	Vector2 position;
	Vector2 previousPosition;
	Vector2 acceleration;
	float mass;
	float friction;

	PointMass(const Vector2 &initialPosition, float mass) : position(initialPosition), previousPosition(initialPosition), acceleration({0.f, 0.f}), mass(mass), friction(0.01f) {}

	Vector2 getVelocity()
	{
		Vector2 velocity;
		velocity.x = position.x - previousPosition.x;
		velocity.y = position.y - previousPosition.y;
		return velocity;
	}

	void update(const float deltaTime)
	{
		// Verlet Integration
		// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
		Vector2 positionComponent;
		Vector2 accelerationComponent;
		Vector2 frictionComponent;

		// TODO: (2.f - damping)?
		positionComponent.x = 2.f * position.x - previousPosition.x;
		positionComponent.y = 2.f * position.y - previousPosition.y;
		accelerationComponent.x = acceleration.x * deltaTime * deltaTime;
		accelerationComponent.y = acceleration.y * deltaTime * deltaTime;
		frictionComponent.x = (position.x - previousPosition.x) * friction * -1.f;
		frictionComponent.y = (position.y - previousPosition.y) * friction * -1.f;

		previousPosition = position;
		position.x = positionComponent.x + accelerationComponent.x;
		position.y = positionComponent.y + accelerationComponent.y;
		position.x = positionComponent.x + accelerationComponent.x + frictionComponent.x;
		position.y = positionComponent.y + accelerationComponent.y + frictionComponent.y;

		acceleration = {0.f, 0.f};
	}
};
