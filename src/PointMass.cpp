#include "PointMass.hpp"

#include "raymath.h"

PointMass::PointMass(const Vector2 &initialPosition, float mass) : Point(initialPosition), mass(mass) {}

PointMass::~PointMass() {}

void PointMass::setMass(const float newMass)
{
	mass = newMass;
}

float PointMass::getMass() const
{
	return mass;
}

void PointMass::satisfyConstraints()
{
	position = Vector2Clamp(position, {0.f, 0.f}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
	// TODO: update previousPosition?
}
