#pragma once

#include "PointMass.hpp"
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>

struct Shape
{
	std::vector<PointMass> vertices;
	Vector2 acceleration;

	Shape() : acceleration({0.f, 0.f}) {}

	void addPointMass(const Vector2 &position, float mass)
	{
		vertices.push_back(PointMass(position, mass));
	}

	void update(float deltaTime)
	{
		for (auto &vertex : vertices)
		{
			vertex.acceleration.x += acceleration.x;
			vertex.acceleration.y += acceleration.y;
			vertex.update(deltaTime);
		}
		acceleration = {0.f, 0.f};
	}

	Vector2 getCenter()
	{
		Vector2 center = {0.f, 0.f};
		for (auto &vertex : vertices)
			center = Vector2Add(center, vertex.position);
		center = Vector2Scale(center, 1.f / vertices.size());
		return center;
	}

	void rotate(float degrees)
	{
		float cosTheta = std::cos(degrees * DEG2RAD);
		float sinTheta = std::sin(degrees * DEG2RAD);
		Vector2 center = getCenter();

		for (auto &vertex : vertices)
		{
			// to make verlet happy: store velocity, then rotate, then recalculate previousPosition to discard angular velocity
			Vector2 velocity = vertex.getVelocity();

			float translatedX = vertex.position.x - center.x;
			float translatedY = vertex.position.y - center.y;

			float rotatedX = cosTheta * translatedX - sinTheta * translatedY;
			float rotatedY = sinTheta * translatedX + cosTheta * translatedY;

			vertex.position.x = rotatedX + center.x;
			vertex.position.y = rotatedY + center.y;

			vertex.previousPosition.x = vertex.position.x - velocity.x;
			vertex.previousPosition.y = vertex.position.y - velocity.y;
		}
	}

	void draw()
	{
		Vector2 *positions = new Vector2[vertices.size() + 1];
		for (size_t i = 0; i < vertices.size(); i++)
			positions[i] = vertices[i].position;
		positions[vertices.size()] = vertices[0].position;

		DrawSplineLinear(positions, vertices.size() + 1, 10.f, BLUE);
	}
};
