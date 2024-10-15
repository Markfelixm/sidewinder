#include "Stick.hpp"

#include "raymath.h"

Stick::Stick(Particle *endA, Particle *endB) : a(endA), b(endB)
{
	restLength = determineDeltaLength();
}

Stick::~Stick() {}

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
