#pragma once

#include "PointMass.hpp"
#include "raymath.h"

struct Spring
{
	PointMass &a;
	PointMass &b;
	float stiffness;
	float damping;
	float restLength;

	Spring(PointMass &a, PointMass &b, float stiffness, float damping)
		: a(a), b(b), stiffness(stiffness), damping(damping), restLength(Vector2Distance(a.position, b.position)) {}

	void apply()
	{
		// TODO: write out operations instead of raymath
		// spring force acting on a and b
		// fa = -fb = - magnitude(k * displacement + damping * dot((va - vb) * normalized(a - b))) * normalized(a - b)
		Vector2 delta = Vector2Subtract(a.position, b.position);
		Vector2 normal = Vector2Normalize(delta);
		float displacement = Vector2Length(delta) - restLength;
		Vector2 relativeVelocity = Vector2Subtract(a.getVelocity(), b.getVelocity());

		float force = -1.f * stiffness * displacement + damping * Vector2DotProduct(relativeVelocity, normal);
		a.acceleration = Vector2Add(a.acceleration, Vector2Scale(normal, force));
		b.acceleration = Vector2Add(b.acceleration, Vector2Scale(normal, -1.f * force));
	}
};
