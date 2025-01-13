#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"
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

		for (auto &spring : springs)
			spring.apply();

		actual.update(deltaTime);
		frame.update(deltaTime);

		// match position and rotation of the frame to actual points
		V2 displacement = actual.getCenter() - frame.getCenter();
		frame.move(displacement);
		float rotationOffset = actual.getRotation() - frame.getRotation();
		frame.rotate(rotationOffset);
	}

	void draw(Color color)
	{
		frame.draw(ColorAlpha(color, 0.2f));
		actual.draw(color);
	}
};
