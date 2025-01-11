#pragma once

#include "PointMass.hpp"
#include "raymath.h"

// TODO: what about a springToMass factor instead of stiffness? multiply end.mass by factor to use in place of stiffness or spring strength
struct FixedSpring
{
	PointMass &anchor;
	PointMass &end;
	float stiffness;
	float restLength;

	FixedSpring(PointMass &anchor, PointMass &end, float stiffness) : anchor(anchor), end(end), stiffness(stiffness), restLength(Vector2Distance(anchor.position, end.position)) {}

	void apply()
	{
		Vector2 delta = {end.position.x - anchor.position.x, end.position.y - anchor.position.y};
		float currentLength = std::sqrt(delta.x * delta.x + delta.y * delta.y);
		if (currentLength == 0.f || end.mass == 0.f)
			return;
		Vector2 direction = {delta.x / currentLength, delta.y / currentLength};
		float displacement = currentLength - restLength;

		Vector2 force = {-direction.x * stiffness * displacement, -direction.y * stiffness * displacement};

		end.acceleration.x += force.x / end.mass;
		end.acceleration.y += force.y / end.mass;
	}
};
