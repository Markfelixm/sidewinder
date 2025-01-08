#pragma once

#include "Shape.hpp"
#include "FixedSpring.hpp"
#include <vector>

struct Framed
{
	Shape frame;
	Shape actual;
	std::vector<FixedSpring> springs;

	Framed(std::vector<PointMass> &points, float stiffness)
	{
		frame = Shape(points);
		frame.setFriction(0.f);
		actual = Shape(points);
		actual.setFriction(0.02f);
		for (size_t i = 0; i < points.size(); i++)
			springs.push_back(FixedSpring(frame.points.at(i), actual.points.at(i), stiffness));
	}

	void update(float deltaTime)
	{
		// match position and rotation of the frame to actual points
		Vector2 displacement = Vector2Subtract(actual.getCenter(), frame.getCenter());
		for (auto &anchor : frame.points)
		{
			anchor.position.x += displacement.x;
			anchor.position.y += displacement.y;
			// anchor.acceleration.x += displacement.x;
			// anchor.acceleration.y += displacement.y;
		}
		float rotationOffset = actual.getRotation() - frame.getRotation();
		frame.rotate(rotationOffset);

		for (auto &spring : springs)
			spring.apply();

		frame.update(deltaTime);
		actual.update(deltaTime);
	}

	void draw(Color color)
	{
		frame.draw(ColorAlpha(color, 0.2f));
		actual.draw(color);
	}
};
