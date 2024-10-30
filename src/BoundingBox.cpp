#include "BoundingBox.hpp"

#include <algorithm>

namespace Shapes
{

	BoundingBox::BoundingBox(const std::vector<PointMass> &pointMasses) : topLeft({0.f, 0.f}), bottomRight({0.f, 0.f})
	{
		resize(pointMasses);
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

	void BoundingBox::resize(const std::vector<PointMass> &pointMasses)
	{
		topLeft = pointMasses.at(0).getPosition();
		bottomRight = pointMasses.at(0).getPosition();

		for (const auto &pointMass : pointMasses)
		{
			topLeft.x = std::min(topLeft.x, pointMass.getPosition().x);
			topLeft.y = std::min(topLeft.y, pointMass.getPosition().y);
			bottomRight.x = std::max(bottomRight.x, pointMass.getPosition().x);
			bottomRight.y = std::max(bottomRight.y, pointMass.getPosition().y);
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
