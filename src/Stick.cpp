#include "Stick.hpp"

#include "raymath.h"

Stick::Stick(Particle *endA, Particle *endB) : a(endA), b(endB), acceleration({0.f, 0.f})
{
	restLength = determineDeltaLength();
}

Stick::~Stick() {}

Vector2 Stick::getAcceleration() const
{
	return acceleration;
}

void Stick::setAcceleration(const Vector2 &netAcceleration)
{
	acceleration = netAcceleration;
	a->setAcceleration(acceleration);
	b->setAcceleration(acceleration);
}

void Stick::satisfyConstraints()
{
	for (int i = 0; i < satisfyIterations; i++)
	{
		a->satisfyConstraints();
		b->satisfyConstraints();

		Vector2 delta = Vector2Subtract(b->getPosition(), a->getPosition());
		float deltaLength = Vector2Length(delta);

		float diffFactor = 0.5 * ((restLength - deltaLength) / deltaLength);
		if (deltaLength == 0.f)
			diffFactor = 0.f;

		a->setPosition(Vector2Subtract(a->getPosition(), Vector2Scale(delta, diffFactor)));
		b->setPosition(Vector2Add(b->getPosition(), Vector2Scale(delta, diffFactor)));
	}
}

float Stick::determineDeltaLength()
{
	Vector2 delta = Vector2Subtract(b->getPosition(), a->getPosition());
	return Vector2Length(delta);
}
