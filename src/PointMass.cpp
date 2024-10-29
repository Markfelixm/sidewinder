#include "PointMass.hpp"

#include "raymath.h"

PointMass::PointMass(const Vector2 &initialPosition, float mass) : position(initialPosition), previousPosition(initialPosition), netAcceleration({0.f, 0.f}), mass(mass) {}

PointMass::~PointMass() {}

void PointMass::setMass(const float newMass)
{
	mass = newMass;
}

float PointMass::getMass() const
{
	return mass;
}

void PointMass::setPosition(const Vector2 &newPosition)
{
	position = newPosition;
}

const Vector2 &PointMass::getPosition() const
{
	return position;
}

const Vector2 PointMass::determineVelocity() const
{
	return Vector2Subtract(position, previousPosition);
}

void PointMass::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
}

const Vector2 &PointMass::getAcceleration() const
{
	return netAcceleration;
}

void PointMass::update(const float deltaTime)
{
	// Verlet Integration
	// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
	Vector2 positionComponent = Vector2Subtract(Vector2Scale(position, 2), previousPosition);
	Vector2 accelerationComponent = Vector2Scale(netAcceleration, deltaTime * deltaTime);
	previousPosition = position;
	position = Vector2Add(positionComponent, accelerationComponent);

	netAcceleration = {0.f, 0.f};
}

void PointMass::draw(const Color &color, const float radius) const
{
	DrawCircleV(position, radius, color);
}

void PointMass::satisfyConstraints()
{
	position = Vector2Clamp(position, {0.f, 0.f}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
	// TODO: update previousPosition?
}
