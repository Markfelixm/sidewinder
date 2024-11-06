#include "BoundingBox.hpp"

#include <algorithm>

namespace Sidewinder
{

	BoundingBox::BoundingBox(const std::vector<Vector2> &vertices) : topLeft({0.f, 0.f}), bottomRight({0.f, 0.f})
	{
		resize(vertices);
	}

	BoundingBox::~BoundingBox() {}

	float BoundingBox::getLeft() const
	{
		return topLeft.x;
	}

	float BoundingBox::getRight() const
	{
		return bottomRight.x;
	}

	float BoundingBox::getTop() const
	{
		return topLeft.y;
	}

	float BoundingBox::getBottom() const
	{
		return bottomRight.y;
	}

	void BoundingBox::resize(const std::vector<Vector2> &vertices)
	{
		topLeft = vertices.at(0);
		bottomRight = vertices.at(0);

		for (const auto &vertex : vertices)
		{
			topLeft.x = std::min(topLeft.x, vertex.x);
			topLeft.y = std::min(topLeft.y, vertex.y);
			bottomRight.x = std::max(bottomRight.x, vertex.x);
			bottomRight.y = std::max(bottomRight.y, vertex.y);
		}
	}

	bool BoundingBox::contains(const Vector2 &point) const
	{
		return (point.x >= topLeft.x && point.x <= bottomRight.x &&
				point.y >= topLeft.y && point.y <= bottomRight.y);
	}

	bool BoundingBox::intersects(const BoundingBox &otherBox) const
	{
		return (topLeft.x <= otherBox.bottomRight.x && bottomRight.x >= otherBox.topLeft.x &&
				topLeft.y <= otherBox.bottomRight.y && bottomRight.y >= otherBox.topLeft.y);
	}

}
