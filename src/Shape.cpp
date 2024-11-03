#include "Shape.hpp"

#include "raymath.h"
#include <cmath>

const Vector2 calculateCenter(const std::vector<Vector2> &vertices);
Vector2 polarToCartesian(const std::pair<float, float> &polar);

Shape::Shape(const std::vector<Vector2> &vertices) : center(calculateCenter(vertices)), previousCenter(center), netAcceleration({0.f, 0.f}), rotation(0.f), previousRotation(0.f)
{
	polars.reserve(vertices.size());
	for (const auto &vertex : vertices)
	{
		Vector2 polarOffset = Vector2Subtract(vertex, center);
		const float radius = Vector2Length(polarOffset);
		float angle = atan2f(polarOffset.y, polarOffset.x);
		polars.emplace_back(radius, angle);
	}
}

Shape::~Shape() {}

const size_t Shape::getVertexCount() const
{
	return polars.size();
}

const std::vector<Vector2> Shape::getVertices() const
{
	std::vector<Vector2> vertices;
	vertices.reserve(polars.size());
	for (const auto &polar : polars)
		vertices.push_back(polarToCartesian(polar));
	return vertices;
}

const Vector2 Shape::getVertexPositionAt(const size_t index) const
{
	return polarToCartesian(polars.at(index));
}

const Vector2 &Shape::getAcceleration() const
{
	return netAcceleration;
}

const Vector2 &Shape::getCenterPosition() const
{
	return center;
}
void Shape::setRotation(const float radians)
{
	rotation = radians;
}

const float Shape::getRotation() const
{
	return rotation;
}

void Shape::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
}

void Shape::updatePolars()
{
	for (auto &polar : polars)
		polar.second -= rotation - previousRotation;
}

void Shape::update(const float deltaTime)
{
	// Verlet Integration
	// x(t + dt) = 2x(t) - x(t - dt) + a * dt * dt
	Vector2 positionComponent = Vector2Subtract(Vector2Scale(center, 2), previousCenter);
	Vector2 accelerationComponent = Vector2Scale(netAcceleration, deltaTime * deltaTime);
	previousCenter = center;
	center = Vector2Add(positionComponent, accelerationComponent);

	updateCenter();
	netAcceleration = {0.f, 0.f};
}

void Shape::satisfyConstraints()
{
}

void Shape::draw(const Color &color, const float thickness) const
{
	if (polars.size() == 0)
		return;

	const std::vector<Vector2> vertices = getVertices();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		DrawLineEx(vertices.at(i), vertices.at((i + 1) % vertices.size()), thickness, color);
		DrawCircleV(vertices.at(i), thickness, color);
	}
	DrawLineEx(vertices.at(vertices.size() - 1), vertices.at(0), thickness, color);
}

void Shape::updateCenter()
{
	center = {0.f, 0.f};
	for (const auto &polar : polars)
		center = Vector2Add(center, polarToCartesian(polar));
	center = Vector2Scale(center, 1.f / polars.size());
}

void Shape::updateRotation()
{
	previousRotation = rotation;
	rotation = 0.f;
	for (const auto &polar : polars)
		rotation += polar.second;
	rotation /= polars.size();
}

Vector2 Shape::polarToCartesian(const std::pair<float, float> &polar) const
{
	return {
		center.x + polar.first * cosf(polar.second),
		center.y + polar.first * sinf(polar.second)};
}

const Vector2 calculateCenter(const std::vector<Vector2> &vertices)
{
	Vector2 averageCenter = {0.f, 0.f};
	for (const auto &vertex : vertices)
		averageCenter = Vector2Add(averageCenter, vertex);
	averageCenter = Vector2Scale(averageCenter, 1.f / vertices.size());
	return averageCenter;
}
