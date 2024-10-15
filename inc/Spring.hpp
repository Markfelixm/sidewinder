#pragma once

#include "raylib.h"

#include "Particle.hpp"

class Spring
{
public:
	Spring(Particle *endA, Particle *endB, float restLength, float stiffness, float damping);
	~Spring();

	void setRestLength(const float newRestLength);
	void setStiffness(const float newStiffness);
	void setDamping(const float newDamping);

	void satisfyConstraints();
	float determineDeltaLength();

private:
	Spring() = delete;

	Particle *a;
	Particle *b;

	float restLength;
	float damping;
	float stiffness;
	static const int satisfyIterations = 1;
};
