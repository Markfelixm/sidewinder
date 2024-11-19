#pragma once

#include "Shape.hpp"
#include "PointMass.hpp"
#include "BoundingBox.hpp"

#include "raylib.h"

#include <vector>
#include <utility>

namespace Sidewinder
{
	class Camera;
}

class SoftBody : public Shape
{
	// TODO: make abstract
public:
	SoftBody(const std::vector<PointMass> &pointMasses);
	~SoftBody();

	const size_t getPointMassCount() const;
	const Vector2 &getPointMassPositionAt(const size_t index) const;
	const std::vector<Vector2> getPointMassPositions() const;
	void setStiffness(const float newStiffness);
	void setDamping(const float newDamping);
	void setColor(const Color &newColor);
	void setIsStationary(bool state);
	const Sidewinder::BoundingBox &getBoundingBox() const;

	virtual void applyAcceleration(const Vector2 &acceleration);
	virtual void update(const float deltaTime);
	virtual void satisfyConstraints();
	virtual void draw(const Sidewinder::Camera &camera) const;

	void moveCenter(const Vector2 newPosition, const float strength);
	void handleSoftBodyCollision(SoftBody &other);
	bool contains(const Vector2 &point) const;
	void resolveCollision(PointMass &collider);
	std::pair<PointMass &, PointMass &> findNearestEdge(const Vector2 &point);

private:
	SoftBody() = delete;

	void updateCenter();
	void updateRotation();

	std::vector<PointMass> pointMasses;
	float stiffness;
	float damping;

	bool isStationary;
	Color color;
	Sidewinder::BoundingBox bounds;
};
