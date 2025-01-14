#include "Viewport.hpp"
#include "Shape.hpp"
#include "Matrix.hpp"

Viewport::Viewport(int width, int height, V2 position, float zoom, float rotation, Shape *target)
	: width(width), height(height), bounds(AABB(0.f, 0.f, 0.f, 0.f)),
	  position(position), zoom(zoom), rotation(rotation), target(target)
{
	update();
}

void Viewport::update()
{
	V2 offset = V2(width, height) * (0.5f / zoom);
	// TODO: attach fixedSpring
	if (target)
		position = target->getCenter();
	position = worldToScreen(position);
	offset = worldToScreen(offset);
	bounds = AABB(
		position.x - offset.x,
		position.x + offset.x,
		position.y - offset.y,
		position.y + offset.y);
}

V2 Viewport::worldToScreen(const V2 &worldPos) const
{
	M22 transform(rotation, zoom);
	V2 offset = transform.transform(worldPos - position);
	return offset + V2(width * 0.5f, height * 0.5f);
}

V2 Viewport::screenToWorld(const V2 &screenPos) const
{
	M22 transform(rotation, zoom);
	V2 offset = screenPos - V2(width * 0.5f, height * 0.5f);
	return position + transform.inverse().transform(offset);
}
