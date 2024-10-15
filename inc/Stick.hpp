#pragma once

#include "raylib.h"

#include "Particle.hpp"

class Stick
{
public:
	Stick(Particle *endA, Particle *endB);
	~Stick();

	void satisfyConstraints();

	float determineDeltaLength();

private:
	Stick() = delete;

	Particle *a;
	Particle *b;

	float restLength;
	static const int satisfyIterations = 1;
};
