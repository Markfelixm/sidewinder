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

	Shape(std::vector<PointMass> &vertices) : vertices(vertices), acceleration({0.f, 0.f}) {}

	void addPointMass(PointMass &pointMass)
	{
		vertices.push_back(pointMass);
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

	void setFriction(float friction)
	{
		for (auto &vertex : vertices)
			vertex.friction = friction;
	}

	Vector2 getCenter()
	{
		Vector2 center = {0.f, 0.f};
		for (auto &vertex : vertices)
			center = Vector2Add(center, vertex.position);
		center = Vector2Scale(center, 1.f / vertices.size());
		return center;
	}

	float getRotation()
	{
		float rotation = 0.f;
		float previous = 0.f;

		Vector2 center = getCenter();
		for (auto &vertex : vertices)
		{
			float dx = vertex.position.x - center.x;
			float dy = vertex.position.y - center.y;
			float angle = std::atan2(dy, dx);

			// avoid discontinuity
			while (angle - previous > M_PI)
				angle -= 2.f * M_PI;
			while (angle - previous < -M_PI)
				angle += 2.f * M_PI;

			rotation += angle;
			previous = angle;
		}
		rotation /= vertices.size();
		return rotation * RAD2DEG;
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

	void draw(Color color)
	{
		Vector2 *positions = new Vector2[vertices.size() + 1];
		for (size_t i = 0; i < vertices.size(); i++)
			positions[i] = vertices[i].position;
		positions[vertices.size()] = vertices[0].position;

		DrawSplineLinear(positions, vertices.size() + 1, 10.f, color);
	}
};
