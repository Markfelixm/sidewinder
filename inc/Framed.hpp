#pragma once

#include "Shape.hpp"
#include "FixedSpring.hpp"
#include <vector>

struct Framed
{
	Shape frame;
	Shape actual;
	std::vector<FixedSpring> springs;

	Framed(std::vector<PointMass> &vertices, float stiffness)
	{
		frame = Shape(vertices);
		frame.setFriction(0.f);
		actual = Shape(vertices);
		actual.setFriction(0.02f);
		for (size_t i = 0; i < vertices.size(); i++)
			springs.push_back(FixedSpring(frame.vertices.at(i), actual.vertices.at(i), stiffness));
	}

	void update(float deltaTime)
	{
		// match position and rotation of the frame to actual vertices
		Vector2 displacement = Vector2Subtract(actual.getCenter(), frame.getCenter());
		for (auto &anchor : frame.vertices)
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

	void draw()
	{
		frame.draw(ColorAlpha(BLUE, 0.2f));
		actual.draw(BLUE);
	}
};
