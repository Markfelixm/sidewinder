#pragma once

#include "Shape.hpp"
#include "PointMass.hpp"

#include "raylib.h"

#include <vector>

class SoftBody : public Shape
{
public:
	SoftBody(const std::vector<PointMass> &pointMasses);
	~SoftBody();

	const size_t getPointMassCount() const;
	const Vector2 &getPointMassPositionAt(const size_t index) const;

	virtual void applyAcceleration(const Vector2 &acceleration);
	virtual void update(const float deltaTime);
	virtual void satisfyConstraints();
	virtual void draw(const Color &color, const float thickness) const;

private:
	SoftBody() = delete;

	std::vector<PointMass> pointMasses;
	// TODO: consider polar coordinate references from center of pointMasses to original positions when constructed. this may make points redundant. polar would be easy to rotate
};
