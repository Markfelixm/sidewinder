#pragma once

#include "Vector.hpp"
#include "AABB.hpp"

struct Shape;

struct Viewport
{
	int width;
	int height;
	AABB bounds;
	V2 position;
	float zoom;
	float rotation;
	Shape *target;

	Viewport(int width, int height, V2 position, float zoom, float rotation, Shape *target = nullptr);
	void update();
	V2 worldToScreen(const V2 &worldPos) const;
	V2 screenToWorld(const V2 &screenPos) const;
};
