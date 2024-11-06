#pragma once

#include "raylib.h"

#include "PointMass.hpp"

#include <vector>

namespace Sidewinder
{

	class BoundingBox
	{
	public:
		BoundingBox(const std::vector<Vector2> &vertices);
		~BoundingBox();

		float getLeft() const;
		float getRight() const;
		float getTop() const;
		float getBottom() const;

		void resize(const std::vector<Vector2> &vertices);
		bool contains(const Vector2 &point) const;
		bool intersects(const BoundingBox &otherBox) const;

	private:
		BoundingBox() = delete;

		Vector2 topLeft;
		Vector2 bottomRight;
	};

}
