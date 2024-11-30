#pragma once

#include "SoftBody.hpp"
#include "Camera.hpp"
#include "raylib.h"
#include <vector>

// TODO: animation system / e.g. moving platforms

class Obstacle : public SoftBody
{
public:
	Obstacle() = delete;
	Obstacle(const std::vector<Vector2> &vertices, const float mass);
	~Obstacle() = default;

	void applyAcceleration(const Vector2 &acceleration) override;
	void update(const float deltaTime) override;
	void satisfyConstraints() override;
	void draw(const Sidewinder::Camera &camera) const override;

private:
};
