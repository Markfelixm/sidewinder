#include "Camera.hpp"

namespace Sidewinder
{

	Camera::Camera(const std::vector<Vector2> &viewCorners) : screenSpace(Sidewinder::BoundingBox(viewCorners))
	{
	}

	Camera::~Camera() {}

	const Sidewinder::BoundingBox &Camera::getBoundingBox() const
	{
		return screenSpace;
	}

	const Vector2 Camera::cameraToWorld(const Vector2 &screenPosition) const
	{
	}

	const Vector2 Camera::worldToCamera(const Vector2 &worldPosition) const
	{
	}

	void Camera::update(const float deltaTime)
	{
		// move camera position with spring attached to target, target is anchor/fixed
	}

	void Camera::drawView() const
	{
		// boundingbox collision all entities and draw ones that collide
	}

}
