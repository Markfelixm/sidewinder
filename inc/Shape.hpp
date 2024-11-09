#pragma once

#include "raylib.h"

#include <vector>
#include <utility>

class Shape
{
public:
	Shape(const std::vector<Vector2> &vertices);
	~Shape();

	const size_t getVertexCount() const;
	const std::vector<Vector2> getVertices() const;
	const Vector2 getVertexPositionAt(const size_t index) const;
	const Vector2 &getAcceleration() const;
	const Vector2 &getCenterPosition() const;
	void setRotation(const float radians);
	const float getRotation() const;

	virtual void applyAcceleration(const Vector2 &acceleration);
	void updatePolars();
	virtual void update(const float deltaTime);
	virtual void satisfyConstraints();

protected:
	virtual void updateCenter();
	virtual void updateRotation();
	Vector2 polarToCartesian(const std::pair<float, float> &polar) const;

	Vector2 center;
	Vector2 previousCenter;
	Vector2 netAcceleration;
	float rotation;
	float previousRotation;
	// polars: polar coordinates of each vertex relative to the center of the shape; radius stays constant to retain original shape, rotations are synchronized. this is my strategy for moving several vertices by managing one central position
	std::vector<std::pair<const float, float>> polars;

private:
	Shape() = delete;
};
