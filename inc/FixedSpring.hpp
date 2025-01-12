#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"

// TODO: what about a springToMass factor instead of stiffness? multiply end.mass by factor to use in place of stiffness or spring strength
struct FixedSpring
{
	PointMass &anchor;
	PointMass &end;
	float stiffness;
	float restLength;

	FixedSpring(PointMass &anchor, PointMass &end, float stiffness) : anchor(anchor), end(end), stiffness(stiffness), restLength((end.position - anchor.position).length()) {}

	void apply()
	{
		V2 delta = end.position - anchor.position;
		float currentLength = delta.length();
		if (currentLength == 0.f || end.mass == 0.f)
			return;
		V2 direction = delta / currentLength;
		float displacement = currentLength - restLength;

		V2 force = direction * stiffness * displacement;

		end.acceleration -= force / end.mass;
	}
};
