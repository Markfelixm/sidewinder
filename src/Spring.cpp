#include "Spring.hpp"

#include "raymath.h"

Spring::Spring(Particle *endA, Particle *endB,
			   float restLength, float stiffness, float damping)
	: a(endA), b(endB),
	  restLength(restLength), stiffness(stiffness), damping(damping) {}

Spring::~Spring() {}

void Spring::setRestLength(const float newRestLength)
{
	restLength = newRestLength;
}

void Spring::setStiffness(const float newStiffness)
{
	stiffness = Clamp(newStiffness, 0.f, 1.f);
}

void Spring::setDamping(const float newDamping)
{
	damping = Clamp(newDamping, 0.f, 0.5f);
}

void Spring::satisfyConstraints()
{
	// damped simple harmonic motion
	Vector2 delta = Vector2Subtract(a->getPosition(), b->getPosition());
	Vector2 direction = Vector2Normalize(delta);
	float displacement = Vector2Length(delta) - restLength;

	Vector2 springForce = Vector2Scale(direction, stiffness * displacement);

	Vector2 aDamping = Vector2Scale(a->determineVelocity(), damping);
	Vector2 aForce = Vector2Subtract(springForce, aDamping);
	a->setPosition(Vector2Subtract(a->getPosition(), aForce));
	Vector2 bDamping = Vector2Scale(b->determineVelocity(), damping);
	Vector2 bForce = Vector2Subtract(springForce, bDamping);
	b->setPosition(Vector2Add(b->getPosition(), bForce));
}

float Spring::determineDeltaLength()
{
	Vector2 delta = Vector2Subtract(b->getPosition(), a->getPosition());
	return Vector2Length(delta);
}
