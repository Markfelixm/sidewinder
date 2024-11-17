#include "Player.hpp"
#include "PointMass.hpp"
#include "Input.hpp"
#include "raymath.h"
#include <vector>
#include <cmath>

std::vector<PointMass> createCircularPointMasses(const Vector2 &center, const size_t pointCount, const float radius, const float mass)
{
	std::vector<PointMass> pointMasses;
	pointMasses.reserve(pointCount);

	for (size_t i = 0; i < pointCount; ++i)
	{
		float angle = (2.f * M_PI * i) / pointCount;
		Vector2 position = {center.x + radius * cosf(angle), center.y + radius * sinf(angle)};
		pointMasses.emplace_back(PointMass(position, mass));
	}
	return pointMasses;
}

Player::Player(Input &input, const Vector2 &position, const size_t pointCount)
	: SoftBody(createCircularPointMasses(position, pointCount, RADIUS, MASS)), input(input)
{
	setColor(GREEN);
	setStiffness(STIFFNESS);
	setDamping(DAMPING);
}

void Player::applyAcceleration(const Vector2 &acceleration)
{
	SoftBody::applyAcceleration(acceleration);
}

void Player::update(const float deltaTime)
{
	handleInput();
	SoftBody::update(deltaTime);
}

void Player::satisfyConstraints()
{
	SoftBody::satisfyConstraints();
}

void Player::draw(const Sidewinder::Camera &camera) const
{
	SoftBody::draw(camera);
}

void Player::handleInput()
{
	Vector2 direction = {0.f, 0.f};
	direction.y -= input.isDown[input.UP];
	direction.y += input.isDown[input.DOWN];
	direction.x -= input.isDown[input.LEFT];
	direction.x += input.isDown[input.RIGHT];
	direction = Vector2Normalize(direction); // TODO: hard code diagonals

	applyAcceleration(Vector2Scale(direction, SPEED));
}
