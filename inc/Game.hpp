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
	float gravity;
	std::vector<PointMass> pointMasses;
	SoftBody *softBody;
	float softBodyStiffness;
	float softBodyDamping;
	Color softBodyColor;
	float dragStrength;
};
