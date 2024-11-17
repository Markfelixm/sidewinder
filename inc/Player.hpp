#pragma once

#include "SoftBody.hpp"

class Input;
namespace Sidewinder
{
	class Camera;
}

class Player : public SoftBody
{
public:
	static constexpr float RADIUS = 50.f;
	static constexpr float MASS = 10.f;
	static constexpr float STIFFNESS = 0.2f;
	static constexpr float DAMPING = 0.2f;
	static constexpr float SPEED = 50.f;

	Player(Input &input, const Vector2 &position, const size_t pointCount);
	~Player() = default;

	void applyAcceleration(const Vector2 &acceleration) override;
	void update(const float deltaTime) override;
	void satisfyConstraints() override;
	void draw(const Sidewinder::Camera &camera) const override;

private:
	Player() = delete;

	Input &input;
	void handleInput();
};
