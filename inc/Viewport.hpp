#pragma once

#include "Vector.hpp"
#include "AABB.hpp"

struct Shape;

struct Viewport
{
	float screenWidth;
	float screenHeight;
	AABB worldBounds;
	V2 centerInWorld;
	Shape *target;
	float zoom;
	float rotation;

	Viewport(int screenWidth, int screenHeight, V2 centerInWorld);
	void update();
	V2 worldToScreen(const V2 &worldPosition) const;
	V2 screenToWorld(const V2 &screenPosition) const;
};
