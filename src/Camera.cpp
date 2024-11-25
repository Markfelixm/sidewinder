#include "Camera.hpp"

#include "raymath.h"

namespace Sidewinder
{

	// TODO: fix GetScreenWidth() and Height()
	Camera::Camera(const World &world) : cameraInWorld({{0.f, 0.f}, {1600.f, 1000.f}})
	{
		cameraCenterInWorld = getCameraCenterInWorld();
		// TODO: add setScale and fix scaling: maybe recalculate boundingBox around center
		scale = {1.f, 1.f};
		rotation = 0.f;
	}

	Camera::~Camera() {}

	const Sidewinder::BoundingBox &Camera::getBoundingBox() const
	{
		return cameraInWorld;
	}

	const Vector2 Camera::getCameraCenterInWorld() const
	{
		return {
			(cameraInWorld.getRight() - cameraInWorld.getLeft()) / 2.f,
			(cameraInWorld.getBottom() - cameraInWorld.getTop()) / 2.f};
	}

	void Camera::setCameraCenterInWorld(const Vector2 &newPositon)
	{
		Vector2 displacement = Vector2Subtract(newPositon, cameraCenterInWorld);
		cameraCenterInWorld = newPositon;
		cameraInWorld.resize({Vector2Add({cameraInWorld.getLeft(), cameraInWorld.getTop()}, displacement),
							  Vector2Add({cameraInWorld.getRight(), cameraInWorld.getBottom()}, displacement)});
	}

	const Vector2 Camera::worldToScreen(const Vector2 &worldPosition) const
	{
		Matrix transform = getTransformationMatrix();

		Vector3 worldVec = {worldPosition.x, worldPosition.y, 0.0f};
		Vector3 screenVec = Vector3Transform(worldVec, transform);

		return {screenVec.x, screenVec.y};
	}

	const Vector2 Camera::screenToWorld(const Vector2 &screenPosition) const
	{
		Matrix transform = MatrixInvert(getTransformationMatrix());

		Vector3 screenVec = {screenPosition.x, screenPosition.y, 0.0f};
		Vector3 worldVec = Vector3Transform(screenVec, transform);

		return {worldVec.x, worldVec.y};
	}

	Matrix Camera::getTransformationMatrix() const
	{
		Matrix transform = MatrixIdentity();

		transform = MatrixMultiply(transform, MatrixTranslate(-1.f * cameraInWorld.getLeft(), -1.f * cameraInWorld.getTop(), 0.0f));
		transform = MatrixMultiply(transform, MatrixRotateZ(rotation * DEG2RAD));
		transform = MatrixMultiply(transform, MatrixScale(scale.x, scale.y, 1.0f));

		return transform;
	}

}
