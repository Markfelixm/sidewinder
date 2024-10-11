#pragma once

#include "raylib.h"

#include "Stick.hpp"

class Game
{
public:
	Game();
	~Game();

	void draw();
	void update(float deltaTime);

private:
	Particle topLeft;
	Particle bottomLeft;
	Particle topRight;
	Particle bottomRight;

	Stick left;
	Stick top;
	Stick right;
	Stick bottom;
	Stick crossLeft;
	Stick crossRight;
};
