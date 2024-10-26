#pragma once

#include "Point.hpp"

#include "raylib.h"

#include <vector>
#include <pair>

class Shape
{
public:
	Shape(const std::vector<Point> &points);
	~Shape();

	const size_t getPointCount() const;
	const Vector2 &getPointPositionAt(const size_t index) const;
	const Vector2 &getAcceleration() const;
	const Vector2 &getCenterPosition() const;
	const float getRotation() const;

	virtual void applyAcceleration(const Vector2 &acceleration);
	virtual void update(const float deltaTime);
	virtual void draw(const Color &color, const float thickness) const;

protected:
	void updateCenter();
	void updateRotation();

	std::vector<Point> points;
	std::vector<std::pair<float, float>> polars;
	Vector2 netAcceleration;
	Vector2 center;
	float rotation;

private:
	Shape() = delete;
};
// TODO: constructor accepts vector<Vector2> of positions. a center point is found and a vector of polar coordinates relative to center are stored. helper functions to handle polar coordinates needed. this removes points array, also removes Point class. PointMass absorbs Point
