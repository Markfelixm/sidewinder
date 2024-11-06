#pragma once

#include "BoundingBox.hpp"
#include "Entity.hpp"

#include "raylib.h"

namespace Sidewinder
{

	class Camera
	{
	public:
		Camera(const std::vector<Vector2> &viewCorners);
		~Camera();

		const Sidewinder::BoundingBox &getBoundingBox() const;

		const Vector2 cameraToWorld(const Vector2 &screenPosition) const;
		const Vector2 worldToCamera(const Vector2 &worldPosition) const;

		void update(const float deltaTime);
		void drawView() const;

	private:
		Camera() = delete;

		// Vector2 position;
		// Vector2 scale;
		// Vector2 translation;
		// Vector2 rotation;

		Sidewinder::BoundingBox screenSpace;
		// std::shared_ptr<Entity> target;
	};
}
