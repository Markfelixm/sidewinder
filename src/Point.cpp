#include "Point.hpp"

#include "raymath.h"

Point::Point(const Vector2 &initialPosition) : position(initialPosition), previousPosition(initialPosition), netAcceleration({0.f, 0.f}) {}

Point::~Point() {}

void Point::setPosition(const Vector2 &newPosition)
{
	position = newPosition;
}

const Vector2 &Point::getPosition() const
{
	return position;
}

void Point::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
}

const Vector2 &Point::getAcceleration() const
{
	return netAcceleration;
}

void Point::update(const float deltaTime)
{
	Vector2 temp = position;

	// Verlet Integration
	// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
	Vector2 positionComponent = Vector2Subtract(Vector2Scale(position, 2), previousPosition);
	Vector2 accelerationComponent = Vector2Scale(netAcceleration, deltaTime * deltaTime);
	position = Vector2Add(positionComponent, accelerationComponent);

	previousPosition = temp;
	netAcceleration = {0.f, 0.f};
}

void Point::draw(const Color &color, const float radius) const
{
	DrawCircleV(position, radius, color);
}
