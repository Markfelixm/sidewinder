#include "SoftBody.hpp"
#include "Point.hpp"

#include "raymath.h"

SoftBody::SoftBody(const std::vector<PointMass> &pointMasses)
	: pointMasses(pointMasses),
	  Shape([&pointMasses]()
			{
			  std::vector<Point> shapePoints;
			  shapePoints.reserve(pointMasses.size());
			  for (const auto &pointMass : pointMasses)
				  shapePoints.push_back(Point(pointMass.getPosition()));
			  return shapePoints; }()) {}

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
	for (size_t i = 0; i < pointMasses.size() && i < points.size(); i++)
	{
		pointMasses[i].applyAcceleration(acceleration);
		points[i].applyAcceleration(acceleration);
	}
}

void SoftBody::update(const float deltaTime)
{
	for (size_t i = 0; i < pointMasses.size() && i < points.size(); i++)
	{
		pointMasses[i].update(deltaTime);
		points[i].update(deltaTime);
	}
	updateCenter();
	updateRotation();
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
	if (pointMasses.size() < 0)
		return;

	pointMasses.at(0).draw(color, thickness);
	DrawLineEx(getPointMassPositionAt(0), getPointPositionAt(0), thickness, ColorAlpha(color, 0.3f));

	if (pointMasses.size() < 2)
		return;

	for (size_t i = 1; i < pointMasses.size(); i++)
	{
		DrawLineEx(getPointMassPositionAt(i - 1), getPointMassPositionAt(i), thickness, color);
		DrawLineEx(getPointMassPositionAt(i), getPointPositionAt(i), thickness, ColorAlpha(color, 0.3f));
		pointMasses.at(i).draw(color, thickness);
	}
	DrawLineEx(getPointMassPositionAt(pointMasses.size() - 1), getPointMassPositionAt(0), thickness, color);
}
