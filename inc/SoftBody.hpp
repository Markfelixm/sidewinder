#pragma once

#include "Shape.hpp"
#include "PointMass.hpp"
#include "BoundingBox.hpp"

#include "raylib.h"

#include <vector>

class SoftBody : public Shape
{
public:
	SoftBody(std::vector<PointMass> &pointMasses);
	~SoftBody();

	const size_t getPointMassCount() const;
	const Vector2 &getPointMassPositionAt(const size_t index) const;
	const std::vector<Vector2> getPointMassPositions() const;
	void setStiffness(const float newStiffness);
	void setDamping(const float newDamping);
	const Shapes::BoundingBox &getBoundingBox() const;

	virtual void applyAcceleration(const Vector2 &acceleration);
	virtual void update(const float deltaTime);
	virtual void satisfyConstraints();
	virtual void draw(const Color &color, const float thickness) const;

	void moveCenter(const Vector2 newPosition, const float strength);
	bool contains(const Vector2 &point) const;

private:
	SoftBody() = delete;

	void updateCenter();
	void updateRotation();

	std::vector<PointMass> pointMasses;
	float stiffness;
	float damping;

	Shapes::BoundingBox bounds;
};
