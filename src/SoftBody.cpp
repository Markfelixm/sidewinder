#include "SoftBody.hpp"

#include "raymath.h"

#include <cmath>

SoftBody::SoftBody(std::vector<PointMass> &pointMasses)
	: Shape([&pointMasses]()
			{
			  std::vector<Vector2> shapeVertices;
			  shapeVertices.reserve(pointMasses.size());
			  for (auto &pointMass : pointMasses)
				  shapeVertices.push_back(pointMass.getPosition());
			  return shapeVertices; }()),
	  pointMasses(pointMasses) {}

SoftBody::~SoftBody() {}

const size_t SoftBody::getPointMassCount() const
{
	return pointMasses.size();
}

const Vector2 &SoftBody::getPointMassPositionAt(const size_t index) const
{
	return pointMasses.at(index).getPosition();
}

void SoftBody::applyAcceleration(const Vector2 &acceleration)
{
	netAcceleration = Vector2Add(netAcceleration, acceleration);
	for (auto &pointMass : pointMasses)
		pointMass.applyAcceleration(acceleration);
}

void SoftBody::update(const float deltaTime)
{
	for (auto &pointMass : pointMasses)
		pointMass.update(deltaTime);
	updateCenter();
	updateRotation();
	rotate();
	netAcceleration = {0.f, 0.f};
}

void SoftBody::satisfyConstraints()
{
	for (auto &pointMass : pointMasses)
		pointMass.satisfyConstraints();
}

void SoftBody::draw(const Color &color, const float thickness) const
{
	Shape::draw(ColorContrast(color, -0.5f), thickness * 0.8f);

	if (pointMasses.size() == 0)
		return;
	pointMasses.at(0).draw(color, thickness);
	DrawLineEx(getPointMassPositionAt(0), getVertexPositionAt(0), thickness, ColorAlpha(color, 0.3f));

	if (polars.size() == 1)
		return;
	const std::vector<Vector2> vertices = getVertices();
	for (size_t i = 1; i < vertices.size(); i++)
	{
		DrawLineEx(getPointMassPositionAt(i - 1), getPointMassPositionAt(i), thickness, color);
		DrawLineEx(getPointMassPositionAt(i), vertices.at(i), thickness, ColorAlpha(color, 0.3f));
		pointMasses.at(i).draw(color, thickness);
	}
	DrawLineEx(getPointMassPositionAt(pointMasses.size() - 1), getPointMassPositionAt(0), thickness, color);
}

void SoftBody::updateCenter()
{
	center = {0.f, 0.f};
	for (const auto &pointMass : pointMasses)
		center = Vector2Add(center, pointMass.getPosition());
	center = Vector2Scale(center, 1.f / pointMasses.size());
}

void SoftBody::updateRotation()
{
	previousRotation = rotation;
	rotation = 0.f;
	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		Vector2 centerToPointMass = Vector2Subtract(getPointMassPositionAt(i), center);
		Vector2 centerToVertex = Vector2Subtract(getVertexPositionAt(i), center);
		float angle = Vector2Angle(centerToPointMass, centerToVertex);
		rotation += angle;
	}
	rotation /= pointMasses.size();
}
