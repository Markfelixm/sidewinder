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

	Shape(const std::vector<PointMass> &points) : points(points), acceleration({0.f, 0.f}), initialRotation(0.f)
	{
		initialRotation = getRotation();
	}

	void addPointMass(PointMass &pointMass)
	{
		points.push_back(pointMass);
		initialRotation = 0.f;
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

	V2 getCenter() const
	{
		V2 center = {0.f, 0.f};
		for (auto &point : points)
			center += point.position;
		center /= points.size();
		return center;
	}

	float getRotation() const
	{
		// TODO: find better way to avoid discontinuity jumps,
		// get average angle of points, convert to degrees, zero initial rotation
		float rotation = 0.f;
		float previous = 0.f;

		V2 center = getCenter();
		for (auto &point : points)
		{
			float angle = (point.position - center).angle();

			// avoid discontinuity
			while (angle - previous > std::numbers::pi)
				angle -= 2.f * std::numbers::pi;
			while (angle - previous < -std::numbers::pi)
				angle += 2.f * std::numbers::pi;

			rotation += angle;
			previous = angle;
		}
		rotation /= points.size();

		rotation = (rotation * 180.f / std::numbers::pi) - initialRotation;

		// remap range for discontinuity fix
		while (rotation < 0.f)
			rotation += 360.f;

		return rotation;
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

	void setRotation(float degrees) { rotate(degrees - getRotation()); }

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

	void draw(Viewport &viewport, Color color) const
	{
		Vector2 *positions = new Vector2[points.size() + 1];
		for (size_t i = 0; i < points.size(); i++)
		{
			positions[i].x = viewport.worldToScreen(points[i].position).x;
			positions[i].y = viewport.worldToScreen(points[i].position).y;
		}
		positions[points.size()].x = viewport.worldToScreen(points[0].position).x;
		positions[points.size()].y = viewport.worldToScreen(points[0].position).y;

		DrawSplineLinear(positions, points.size() + 1, 10.f * viewport.zoom, color);
	}
};
