#include "Shape.hpp"

#include "raymath.h"

Shape::Shape(const std::vector<Point> &points)
{
	this->points.reserve(points.size());
	this->points = points;
	updateCenter();
	updateRotation();
}

Shape::~Shape() {}

const size_t Shape::getPointCount() const
{
	return points.size();
}

const Vector2 &Shape::getPointPositionAt(const size_t index) const
{
	return points.at(index).getPosition();
}

const Vector2 &Shape::getAcceleration() const
{
	return netAcceleration;
}

const Vector2 &Shape::getCenterPosition() const
{
	return center;
}

const float Shape::getRotation() const
{
	return rotation;
}

void Shape::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
	for (auto point : points)
		point.applyAcceleration(acceleration);
}

void Shape::update(const float deltaTime)
{
	for (auto point : points)
		point.update(deltaTime);
	updateCenter();
	updateRotation();
	netAcceleration = {0.f, 0.f};
}

void Shape::draw(const Color &color, const float thickness) const
{
	if (points.size() < 0)
		return;

	points.at(0).draw(color, thickness);

	if (points.size() < 2)
		return;

	for (size_t i = 1; i < points.size(); i++)
	{
		DrawLineEx(points.at(i - 1).getPosition(), points.at(i).getPosition(), thickness, color);
		points.at(i).draw(color, thickness);
	}
	DrawLineEx(points.at(points.size() - 1).getPosition(), points.at(0).getPosition(), thickness, color);

	DrawCircleV(center, thickness * 1.2f, ColorBrightness(color, 0.8f));
	DrawLineEx(center, Vector2Rotate(Vector2Scale(Vector2Normalize(center), 20.f), rotation), 10.f, GREEN);
}

void Shape::updateCenter()
{
	center = {0.f, 0.f};
	for (auto point : points)
		center = Vector2Add(center, point.getPosition());
	center = Vector2Scale(center, 1.f / points.size());
}

void Shape::updateRotation()
{
	float rotation = 0.f;
	for (auto point : points)
		rotation += Vector2LineAngle(Vector2Normalize(center), Vector2Normalize(point.getPosition()));
	rotation = rotation / points.size();
}
