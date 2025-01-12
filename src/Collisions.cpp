#include "Collisions.hpp"
#include <cmath>
#include <limits>

bool collides(Shape &attacker, Shape &defender)
{
	// TODO: AABB check OR RATHER sweep and prune beforehand
	for (auto &point : attacker.points)
	{
		if (contains(defender.points, point.position))
			return true;
	}

	return false;
}

bool contains(std::vector<PointMass> &container, V2 &position)
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

void handleCollision(Shape &attacker, Shape &defender)
{
	for (auto &point : attacker.points)
	{
		if (contains(defender.points, point.position))
			resolve(defender, point);
	}
}

void resolve(Shape &defender, PointMass &collider)
{
	// TODO: avoid recalculating
	Edge nearest = findNearestEdge(defender.points, collider.position);
	V2 projection = projectOntoEdge(nearest, collider.position);
	V2 resolution = projection - collider.position;

	float edgeMassRatio = collider.mass / (collider.mass + nearest.a.mass + nearest.b.mass);
	float colliderMassRatio = 1.f - edgeMassRatio;
	float t = relativePosition(nearest, collider.position);
	float aMassRatio = (1.f - t) * edgeMassRatio;
	float bMassRatio = t * edgeMassRatio;

	collider.position += resolution * colliderMassRatio;

	nearest.a.position -= resolution * aMassRatio;
	nearest.b.position -= resolution * bMassRatio;
}

bool isHorizontalIntersect(const V2 &point, const V2 &a, const V2 &b)
{
	V2 lower = a;
	V2 upper = b;

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
	return intersectX > point.x;
}

float relativePosition(const Edge &edge, const V2 &position)
{
	// the progress of projection along edge, from 0 to 1
	V2 ab = edge.b.position - edge.a.position;
	V2 ap = position - edge.a.position;

	float t = ap.dot(ab) / ab.lengthSquared();

	return std::fmax(0.f, std::fmin(1.f, t));
}

V2 projectOntoEdge(const Edge &edge, const V2 &position)
{
	float t = relativePosition(edge, position);
	V2 ab = edge.b.position - edge.a.position;

	return edge.a.position + (ab * t);
}

Edge findNearestEdge(std::vector<PointMass> &points, V2 &position)
{
	float minDistance = std::numeric_limits<float>::max();
	size_t nearestEdgeIndex = 0;

	for (size_t i = 0; i < points.size(); i++)
	{
		Edge edge = {points[i], points[(i + 1) % points.size()]};

		V2 projection = projectOntoEdge(edge, position);
		float distance = (position - projection).lengthSquared();

		if (distance < minDistance)
		{
			minDistance = distance;
			nearestEdgeIndex = i;
		}
	}

	return Edge(points[nearestEdgeIndex], points[(nearestEdgeIndex + 1) % points.size()]);
}
