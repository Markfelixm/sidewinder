#pragma once

#include "PointMass.hpp"
#include "Shape.hpp"
#include <vector>

struct Edge
{
	PointMass &a;
	PointMass &b;

	Edge(PointMass &edgeA, PointMass &edgeB) : a(edgeA), b(edgeB) {}
};

bool collides(Shape &attacker, Shape &defender);
bool contains(std::vector<PointMass> &container, Vector2 &position);
void handleCollision(Shape &attacker, Shape &defender);
void resolve(Shape &defender, PointMass &collider);
bool isHorizontalIntersect(Vector2 &point, Vector2 &a, Vector2 &b);
float relativePosition(const Edge &edge, const Vector2 &position);
Vector2 projectOntoEdge(const Edge &edge, const Vector2 &position);
float distanceSquaredToEdge(const Edge &edge, const Vector2 &position);
Edge findNearestEdge(std::vector<PointMass> &points, Vector2 &position);
