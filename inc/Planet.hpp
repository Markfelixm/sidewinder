#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"
#include "Framed.hpp"
#include <vector>
#include <numbers>

struct Planet
{
	static constexpr float G = 10000.f;
	float radius;
	float mass;
	Framed planet;

	Planet(const V2 &center, float radius, float mass, size_t pointCount) : radius(radius), mass(mass), planet(Framed(createPointMassCircle(center, pointCount), mass)) {}

	std::vector<PointMass> createPointMassCircle(const V2 &center, size_t pointCount)
	{
		std::vector<PointMass> pointMasses;
		pointMasses.reserve(pointCount);

		if (radius < 1.f)
			radius = 1.f;
		if (pointCount < 3)
			pointCount = 3;

		for (size_t i = 0; i < pointCount; i++)
		{
			float angle = (2.f * std::numbers::pi * i) / pointCount;
			V2 position = {center.x + radius * cosf(angle), center.y + radius * sinf(angle)};
			pointMasses.emplace_back(PointMass(position, mass / pointCount));
		}
		return pointMasses;
	}

	float gravityAtDistance(float distance) const
	{
		if (distance == 0.f)
			return 0.f;
		if (distance <= radius)
			return G * mass / (radius * radius) * (distance / radius);
		return G * mass / (distance * distance);
	}

	V2 gravitationalForce(const V2 &position) const
	{
		V2 direction = planet.actual.getCenter() - position;
		float distance = direction.length();

		float gravity = gravityAtDistance(distance);
		direction = direction.normalize();

		return direction * gravity;
	}

	void update(float deltaTime)
	{
		planet.update(deltaTime);
	}

	void draw(Viewport &viewport, Color color) const
	{
		planet.draw(viewport, color);
	}
};
