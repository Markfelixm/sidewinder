#pragma once

#include "raylib.h"

#include "Spring.hpp"

class Game
{
public:
	Game();
	~Game();

	void draw();
	void update(float deltaTime);

private:
	void mouseDragBob();
	void mouseDragBox();

	float gravity;
	float particleRadius;

	Particle anchor;
	Particle bob;
	Particle a;
	Particle b;
	Particle c;
	Particle d;

	float springStiffness;
	float springDamping;

	Spring anchoredSpring;
	Spring springAB;
	Spring springBC;
	Spring springCD;
	Spring springAD;
	Spring springAC;
	Spring springBD;
};
