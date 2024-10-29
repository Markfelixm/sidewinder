#pragma once

#include "PointMass.hpp"
#include "SoftBody.hpp"
#include "Shape.hpp"

#include "raylib.h"

#include <vector>

class Game
{
public:
	Game();
	~Game();

	void draw();
	void update(float deltaTime);

private:
	SoftBody *softBody;
	Shape *shape;
	float gravity;
	PointMass *point;
	std::vector<PointMass> pointMasses;
};
