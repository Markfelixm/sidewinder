#include "Collisions.hpp"

static bool isHorizontalIntersect(Vector2 &point, Vector2 &a, Vector2 &b);

bool collides(Shape &attacker, Shape &defender)
{
	return true;
}

bool contains(Vector2 &position, std::vector<PointMass> &container)
{
	// horizontal even-odd collision check
	int intersectionCount = 0;

	for (size_t i = 0; i < container.size(); i++)
	{
		// check if point to horizontal right intersects neighboring vertices
		if (isHorizontalIntersect(position,
								  container.at(i).position,
								  container.at((i + 1) % container.size()).position))
			intersectionCount++;
	}

	return (intersectionCount % 2) == 1;
}

static bool isHorizontalIntersect(Vector2 &point, Vector2 &a, Vector2 &b)
{
	Vector2 lower = a;
	Vector2 upper = b;

	if (a.y > b.y)
	{
		lower = b;
		upper = a;
	}

	if (point.y < lower.y || point.y > upper.y || (upper.y - lower.y) == 0.f)
		return false;

	// for horizontal line against non-horizontal line
	// x-intersect = lower.x + (y-intersect - lower.y) * dx/dy
	// where y-intersect is known as point.y
	float intersectX = lower.x + (point.y - lower.y) * (upper.x - lower.x) / (upper.y - lower.y);

	// if x-intersect is to the right of point.x the lines intersect
	return intersectX > point.x;
}

void resolve(PointMass &vertex, PointMass &edgeA, PointMass &edgeB)
{
}
