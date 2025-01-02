#pragma once

#include "PointMass.hpp"
#include "FixedSpring.hpp"
#include <vector>

struct Frame
{
	Vector2 center;
	std::vector<PointMass> vertices;
	std::vector<FixedSpring> springs;

	Frame(std::vector<PointMass> &shapeVertices, float stiffness)
	{
		vertices.reserve(shapeVertices.size());
		springs.reserve(shapeVertices.size());
		for (size_t i = 0; i < shapeVertices.size(); i++)
		{
			PointMass copied = shapeVertices[i];
			vertices.push_back(copied);
			springs.push_back(FixedSpring(copied, shapeVertices.at(i), stiffness));
		}
	}

	void apply()
	{
		for (auto &spring : springs)
			spring.apply();
	}
};
