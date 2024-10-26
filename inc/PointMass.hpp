#pragma once

#include "Point.hpp"

#include "raylib.h"

class PointMass : public Point
{
public:
	PointMass(const Vector2 &initialPosition, float mass);
	~PointMass();

	void setMass(const float newMass);
	float getMass() const;

	// void update(const float deltaTime);
	virtual void satisfyConstraints();

private:
	PointMass() = delete;

	float mass;
};
