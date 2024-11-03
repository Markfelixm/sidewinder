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
	Vector2 gravity;
	float gravityStrength;
	std::vector<PointMass> heartMasses;
	std::unique_ptr<SoftBody> heart;
	std::vector<PointMass> boxMasses;
	std::unique_ptr<SoftBody> box;
	float softBodyStiffness;
	float softBodyDamping;
	Color softBodyColor;

	PointMass collider;
	float colliderMass;
};
