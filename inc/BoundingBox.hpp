#pragma once

#include "raylib.h"

#include "PointMass.hpp"

#include <vector>

namespace Shapes
{

	class BoundingBox
	{
	public:
		BoundingBox(const std::vector<PointMass> &pointMasses);
		~BoundingBox();

		void resize(const std::vector<PointMass> &pointMasses);
		bool contains(const Vector2 &point) const;
		bool intersects(const BoundingBox &otherBox) const;

	private:
		BoundingBox() = delete;

		Vector2 topLeft;
		Vector2 bottomRight;
	};

}
