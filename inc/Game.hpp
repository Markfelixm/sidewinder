#pragma once

#include "Ball.hpp"

#include "raylib.h"

class Game
{
public:
	Game();
	~Game();

	void draw();
	void update(float dt);

private:
	static const int numBalls = 500;
	Ball *balls[numBalls];
};
