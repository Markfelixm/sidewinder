#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"
#include "Viewport.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <numbers>

struct Shape
{
	std::vector<PointMass> points;
	V2 acceleration;
	float initialRotation;

	Shape() : acceleration({0.f, 0.f}), initialRotation(0.f) {}

	Shape(std::vector<PointMass> &points) : points(points), acceleration({0.f, 0.f}), initialRotation(getRotation()) {}

	void addPointMass(PointMass &pointMass)
	{
		points.push_back(pointMass);
		initialRotation = getRotation();
	}

	void update(float deltaTime)
	{
		for (auto &point : points)
		{
			point.acceleration += acceleration;
			point.update(deltaTime);
		}
		acceleration = {0.f, 0.f};
	}

	void setFriction(float friction)
	{
		for (auto &point : points)
			point.friction = friction;
	}

	V2 getCenter()
	{
		V2 center = {0.f, 0.f};
		for (auto &point : points)
			center += point.position;
		center /= points.size();
		return center;
	}

	float getRotation()
	{
		// TODO: fix: should return 0 - 360 range
		float rotation = 0.f;
		float previous = 0.f;

		V2 center = getCenter();
		for (auto &point : points)
		{
			float angle = (point.position - center).angle();

			// avoid discontinuity
			while (angle - previous > M_PI)
				angle -= 2.f * M_PI;
			while (angle - previous < -M_PI)
				angle += 2.f * M_PI;

			rotation += angle;
			previous = angle;
		}
		rotation /= points.size();

		return rotation * 180.f / std::numbers::pi;
	}

	void rotate(float degrees)
	{
		float radians = degrees * std::numbers::pi / 180.f;
		float cosTheta = std::cos(radians);
		float sinTheta = std::sin(radians);
		V2 center = getCenter();

		for (auto &point : points)
		{
			// to make verlet happy: store velocity, then rotate, then recalculate previousPosition to discard angular velocity
			V2 velocity = point.getVelocity();

			V2 translated = point.position - center;
			V2 rotated = {cosTheta * translated.x - sinTheta * translated.y, sinTheta * translated.x + cosTheta * translated.y};

			point.position = rotated + center;
			point.previousPosition = point.position - velocity;
		}
	}

	void setRotation(float degrees)
	{
		float offset = degrees - getRotation() + initialRotation;
		rotate(offset);
	}

	void move(V2 &displacement)
	{
		for (auto &point : points)
			point.position += displacement;
	}

	void setPosition(V2 &position)
	{
		V2 displacement = position - getCenter();

		for (auto &point : points)
		{
			point.position += displacement;
			point.previousPosition = point.position;
		}
	}

	void draw(Viewport &viewport, Color color)
	{
		Vector2 *positions = new Vector2[points.size() + 1];
		for (size_t i = 0; i < points.size(); i++)
		{
			positions[i].x = viewport.worldToScreen(points[i].position).x;
			positions[i].y = viewport.worldToScreen(points[i].position).y;
		}
		positions[points.size()].x = viewport.worldToScreen(points[0].position).x;
		positions[points.size()].y = viewport.worldToScreen(points[0].position).y;

		DrawSplineLinear(positions, points.size() + 1, 10.f, color);
	}
};
