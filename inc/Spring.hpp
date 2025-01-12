#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"

struct Spring
{
	PointMass &a;
	PointMass &b;
	float stiffness;
	float damping;
	float restLength;

	Spring(PointMass &a, PointMass &b, float stiffness, float damping)
		: a(a), b(b), stiffness(stiffness), damping(damping), restLength((b.position - a.position).length()) {}

	void apply()
	{
		// spring force acting on a and b
		// fa = -fb = - magnitude(k * displacement + damping * dot((va - vb) * normalized(a - b))) * normalized(a - b)
		V2 delta = a.position - b.position;
		V2 unit = delta.normalize();
		float displacement = delta.length() - restLength;
		V2 relativeVelocity = a.getVelocity() - b.getVelocity();

		float force = stiffness * displacement + damping * relativeVelocity.dot(unit);
		a.acceleration -= unit * force;
		b.acceleration += unit * force;
	}
};
