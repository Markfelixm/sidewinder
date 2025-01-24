#include "Viewport.hpp"
#include "Shape.hpp"
#include "Matrix.hpp"

Viewport::Viewport(int screenWidth, int screenHeight, V2 centerInWorld)
	: screenWidth(screenWidth), screenHeight(screenHeight),
	  worldBounds(AABB(0.f, 0.f, 0.f, 0.f)),
	  centerInWorld(centerInWorld), target(nullptr),
	  zoom(1.f), rotation(0.f)
{
	update();
}

void Viewport::update()
{
	// TODO: attach fixedSpring
	if (target)
		centerInWorld = target->getCenter();

	// TODO: add splitscreen, where these coords can be screenbox aabb instead of 0,0 to width,height
	std::vector<V2> worldPoints;
	worldPoints.reserve(4);
	worldPoints.push_back(screenToWorld({0.f, 0.f}));
	worldPoints.push_back(screenToWorld({screenWidth, 0.f}));
	worldPoints.push_back(screenToWorld({screenWidth, screenHeight}));
	worldPoints.push_back(screenToWorld({0.f, screenHeight}));

	worldBounds.resize(worldPoints);
}

V2 Viewport::worldToScreen(const V2 &worldPosition) const
{
	M22 transform(rotation, zoom);
	V2 offset = transform.transform(worldPosition - centerInWorld);
	return offset + V2(screenWidth * 0.5f, screenHeight * 0.5f);
}

V2 Viewport::screenToWorld(const V2 &screenPosition) const
{
	M22 transform(rotation, zoom);
	V2 offset = screenPosition - V2(screenWidth * 0.5f, screenHeight * 0.5f);
	return centerInWorld + transform.inverse().transform(offset);
}
