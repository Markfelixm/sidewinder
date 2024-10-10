#pragma once

#include "Particle.hpp"

#include "raylib.h"

class Ball : public Particle
{
public:
	Ball(const Vector2 &initialPosition, float radius, float mass);
	~Ball();

	float getRadius() const;
	float getMass() const;

	bool isCollision(const Ball &other) const;
	void resolveCollision(Ball &other);

private:
	Ball() = delete;

	float radius;
	float mass;
};
