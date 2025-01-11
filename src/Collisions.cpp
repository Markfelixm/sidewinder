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

bool contains(std::vector<PointMass> &container, Vector2 &position)
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
	Vector2 projection = projectOntoEdge(nearest, collider.position);
	Vector2 resolution = {projection.x - collider.position.x, projection.y - collider.position.y};

	float edgeMassRatio = collider.mass / (collider.mass + nearest.a.mass + nearest.b.mass);
	float colliderMassRatio = 1.f - edgeMassRatio;
	float t = relativePosition(nearest, collider.position);
	float aMassRatio = (1.f - t) * edgeMassRatio;
	float bMassRatio = t * edgeMassRatio;

	collider.position.x += resolution.x * colliderMassRatio;
	collider.position.y += resolution.y * colliderMassRatio;

	nearest.a.position.x -= resolution.x * aMassRatio;
	nearest.a.position.y -= resolution.y * aMassRatio;
	nearest.b.position.x -= resolution.x * bMassRatio;
	nearest.b.position.y -= resolution.y * bMassRatio;
}

bool isHorizontalIntersect(Vector2 &point, Vector2 &a, Vector2 &b)
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
	return intersectX > point.x;
}

float relativePosition(const Edge &edge, const Vector2 &position)
{
	// the progress of projection along edge, from 0 to 1
	Vector2 ab = {edge.b.position.x - edge.a.position.x, edge.b.position.y - edge.a.position.y};
	Vector2 ap = {position.x - edge.a.position.x, position.y - edge.a.position.y};

	float edgeLengthSquared = ab.x * ab.x + ab.y * ab.y;
	float dot = ap.x * ab.x + ap.y * ab.y;
	float t = dot / edgeLengthSquared;

	return std::fmax(0.f, std::fmin(1.f, t));
}

Vector2 projectOntoEdge(const Edge &edge, const Vector2 &position)
{
	float t = relativePosition(edge, position);

	Vector2 ab = {edge.b.position.x - edge.a.position.x, edge.b.position.y - edge.a.position.y};

	return {
		edge.a.position.x + t * ab.x,
		edge.a.position.y + t * ab.y};
}

float distanceSquaredToEdge(const Edge &edge, const Vector2 &position)
{
	Vector2 projection = projectOntoEdge(edge, position);
	Vector2 delta = {position.x - projection.x, position.y - projection.y};

	return delta.x * delta.x + delta.y * delta.y;
}

Edge findNearestEdge(std::vector<PointMass> &points, Vector2 &position)
{
	float minDistance = std::numeric_limits<float>::max();
	size_t nearestEdgeIndex = 0;

	for (size_t i = 0; i < points.size(); i++)
	{
		Edge edge = {points[i], points[(i + 1) % points.size()]};
		float distance = distanceSquaredToEdge(edge, position);

		if (distance < minDistance)
		{
			minDistance = distance;
			nearestEdgeIndex = i;
		}
	}

	return Edge(points[nearestEdgeIndex], points[(nearestEdgeIndex + 1) % points.size()]);
}
