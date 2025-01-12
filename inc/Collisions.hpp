#pragma once

#include "Vector.hpp"
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
bool contains(std::vector<PointMass> &container, V2 &position);
void handleCollision(Shape &attacker, Shape &defender);
void resolve(Shape &defender, PointMass &collider);
bool isHorizontalIntersect(const V2 &point, const V2 &a, const V2 &b);
float relativePosition(const Edge &edge, const V2 &position);
V2 projectOntoEdge(const Edge &edge, const V2 &position);
Edge findNearestEdge(std::vector<PointMass> &points, V2 &position);
