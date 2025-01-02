#pragma once

#include "PointMass.hpp"
#include "raymath.h"

struct FixedSpring
{
	PointMass &anchor;
	PointMass &end;
	float stiffness;
	float restLength;

	FixedSpring(PointMass &anchor, PointMass &end, float stiffness) : anchor(anchor), end(end), stiffness(stiffness), restLength(Vector2Distance(anchor.position, end.position)) {}

	void apply()
	{
		// TODO: write out operations instead of raymath
		Vector2 delta = Vector2Subtract(end.position, anchor.position);
		float currentLength = Vector2Length(delta);

		Vector2 direction = Vector2Normalize(delta);
		float displacement = currentLength - restLength;
		Vector2 force = Vector2Scale(direction, -stiffness * displacement);

		Vector2 acceleration = Vector2Scale(force, 1.0f / end.mass);
		end.acceleration = Vector2Add(end.acceleration, acceleration);

		// TODO: damping?
		// Vector2 velocity = end.getVelocity();
		// float dampingFactor = 0.1f; // Adjust as needed
		// Vector2 dampingForce = Vector2Scale(velocity, -dampingFactor);
		// end.acceleration = Vector2Add(end.acceleration, Vector2Scale(dampingForce, 1.0f / end.mass));
	}
};
