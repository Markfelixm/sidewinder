#pragma once

#include "World.hpp"
#include "BoundingBox.hpp"
#include "raylib.h"

namespace Sidewinder
{
	class Camera
	{
	public:
		Camera(const World &world);
		~Camera();

		const Sidewinder::BoundingBox &getBoundingBox() const;
		const Vector2 getCameraCenterInWorld() const;
		void setCameraCenterInWorld(const Vector2 &newPosition);

		const Vector2 screenToWorld(const Vector2 &screenPosition) const;
		const Vector2 worldToScreen(const Vector2 &worldPosition) const;

	private:
		Camera() = delete;

		Vector2 cameraCenterInWorld;
		Vector2 scale;
		float rotation;

		Sidewinder::BoundingBox cameraInWorld;

		Matrix getTransformationMatrix() const;
	};
}
