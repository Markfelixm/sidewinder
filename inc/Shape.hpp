#pragma once

#include "PointMass.hpp"
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>

struct Shape
{
	std::vector<PointMass> points;
	Vector2 acceleration;

	Shape() : acceleration({0.f, 0.f}) {}

	Shape(std::vector<PointMass> &points) : points(points), acceleration({0.f, 0.f}) {}

	void addPointMass(PointMass &pointMass)
	{
		points.push_back(pointMass);
	}

	void update(float deltaTime)
	{
		for (auto &point : points)
		{
			point.acceleration.x += acceleration.x;
			point.acceleration.y += acceleration.y;
			point.update(deltaTime);
		}
		acceleration = {0.f, 0.f};
	}

	void setFriction(float friction)
	{
		for (auto &point : points)
			point.friction = friction;
	}

	Vector2 getCenter()
	{
		Vector2 center = {0.f, 0.f};
		for (auto &point : points)
			center = Vector2Add(center, point.position);
		center = Vector2Scale(center, 1.f / points.size());
		return center;
	}

	float getRotation()
	{
		float rotation = 0.f;
		float previous = 0.f;

		Vector2 center = getCenter();
		for (auto &point : points)
		{
			float dx = point.position.x - center.x;
			float dy = point.position.y - center.y;
			float angle = std::atan2(dy, dx);

			// avoid discontinuity
			while (angle - previous > M_PI)
				angle -= 2.f * M_PI;
			while (angle - previous < -M_PI)
				angle += 2.f * M_PI;

			rotation += angle;
			previous = angle;
		}
		rotation /= points.size();
		return rotation * RAD2DEG;
	}

	void rotate(float degrees)
	{
		float cosTheta = std::cos(degrees * DEG2RAD);
		float sinTheta = std::sin(degrees * DEG2RAD);
		Vector2 center = getCenter();

		for (auto &point : points)
		{
			// to make verlet happy: store velocity, then rotate, then recalculate previousPosition to discard angular velocity
			Vector2 velocity = point.getVelocity();

			float translatedX = point.position.x - center.x;
			float translatedY = point.position.y - center.y;

			float rotatedX = cosTheta * translatedX - sinTheta * translatedY;
			float rotatedY = sinTheta * translatedX + cosTheta * translatedY;

			point.position.x = rotatedX + center.x;
			point.position.y = rotatedY + center.y;

			point.previousPosition.x = point.position.x - velocity.x;
			point.previousPosition.y = point.position.y - velocity.y;
		}
	}

	void draw(Color color)
	{
		Vector2 *positions = new Vector2[points.size() + 1];
		for (size_t i = 0; i < points.size(); i++)
			positions[i] = points[i].position;
		positions[points.size()] = points[0].position;

		DrawSplineLinear(positions, points.size() + 1, 10.f, color);
	}
};
