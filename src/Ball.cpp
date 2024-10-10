#include "Ball.hpp"

#include <raymath.h>

Ball::Ball(const Vector2 &initialPosition, float radius, float mass)
	: Particle(initialPosition), radius(radius), mass(mass) {}

Ball::~Ball() {}

float Ball::getRadius() const
{
	return radius;
}

float Ball::getMass() const
{
	return mass;
}

bool Ball::isCollision(const Ball &other) const
{
	float distance = Vector2Distance(getPosition(), other.getPosition());
	return distance < (radius + other.getRadius());
}

void Ball::resolveCollision(Ball &other)
{
	static const float elasticity = 0.7f;

	// determine collision overlap
	Vector2 delta = Vector2Subtract(getPosition(), other.getPosition());
	float distance = Vector2Length(delta);
	float overlap = (radius + other.getRadius()) - distance;

	if (distance != 0)
	{
		// determine correction proptional to mass
		float totalMass = getMass() + other.getMass();
		if (totalMass == 0)
			totalMass = __FLT_EPSILON__;
		float massRatio = getMass() / totalMass;

		float correction = (1.f - massRatio) * overlap * elasticity;
		float otherCorrection = massRatio * overlap * elasticity;
		// more realistic
		// float correction = massRatio * overlap;
		// float otherCorrection = (1.f - massRatio) * overlap;

		// apply correction to each ball in opposing directions
		Vector2 normal = Vector2Normalize(delta);
		setPosition(Vector2Add(getPosition(), Vector2Scale(normal, correction)));
		other.setPosition(Vector2Subtract(other.getPosition(), Vector2Scale(normal, otherCorrection)));
	}
}
