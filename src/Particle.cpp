#include "Particle.hpp"

#include "raymath.h"

Particle::Particle(const Vector2 &initialPosition) : position(initialPosition), previousPosition(initialPosition), netAcceleration({0.f, 0.f}) {}

Particle::~Particle() {}

Vector2 Particle::getPosition() const
{
	return position;
}

void Particle::setPosition(const Vector2 &newPosition)
{
	position = newPosition;
}

Vector2 Particle::getAcceleration() const
{
	return netAcceleration;
}

void Particle::setAcceleration(const Vector2 &acceleration)
{
	netAcceleration = acceleration;
}

void Particle::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
}

void Particle::update(float deltaTime)
{
	Vector2 temp = position;

	// Verlet Integration
	// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
	Vector2 positionComponent = Vector2Subtract(Vector2Scale(position, 2), previousPosition);
	Vector2 accelerationComponent = Vector2Scale(netAcceleration, deltaTime * deltaTime);
	position = Vector2Add(positionComponent, accelerationComponent);

	previousPosition = temp;
}

void Particle::satisfyConstraints()
{
	position = Vector2Clamp(position, {0.f, 0.f}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
}

Vector2 Particle::determineVelocity() const
{
	return Vector2Subtract(position, previousPosition);
}
