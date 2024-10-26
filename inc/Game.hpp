#pragma once

#include "Point.hpp"
#include "PointMass.hpp"
#include "SoftBody.hpp"

#include "raylib.h"

class Game
{
public:
	Game();
	~Game();

	void draw();
	void update(float deltaTime);

private:
	SoftBody *softBody;
	float gravity;
};
