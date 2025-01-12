#pragma once

#include "Vector.hpp"
#include "PointMass.hpp"
#include "raylib.h"
#include <vector>

struct AABB
{
	float left;
	float right;
	float top;
	float bottom;

	AABB(std::vector<PointMass> &vertices)
	{
		resize(vertices);
	}

	void resize(std::vector<PointMass> &vertices)
	{
		left = std::numeric_limits<float>::max();
		right = std::numeric_limits<float>::lowest();
		top = std::numeric_limits<float>::max();
		bottom = std::numeric_limits<float>::lowest();

		for (const auto &vertex : vertices)
		{
			if (vertex.position.x < left)
				left = vertex.position.x;
			if (vertex.position.x > right)
				right = vertex.position.x;
			if (vertex.position.y < top)
				top = vertex.position.y;
			if (vertex.position.y > bottom)
				bottom = vertex.position.y;
		}
	}

	bool contains(V2 &position)
	{
		return (position.x >= left && position.x <= right &&
				position.y >= top && position.y <= bottom);
	}
	bool intersects(AABB &other)
	{
		return !(right < other.left || left > other.right || bottom < other.top || top > other.bottom);
	}

	void draw(Color color)
	{
		DrawRectangle(left, top, right - left, bottom - top, color);
	}
};
