#include "SoftBody.hpp"

#include "raymath.h"

bool intersects(const Vector2 &point, const Vector2 &p1, const Vector2 &p2);

Vector2 calculateSpringForce(
	const Vector2 &currentPosition,
	const Vector2 &targetPosition,
	const Vector2 &currentVelocity,
	float damping,
	float stiffness);

SoftBody::SoftBody(std::vector<PointMass> &pointMasses)
	: Shape([&pointMasses]()
			{
			  std::vector<Vector2> shapeVertices;
			  shapeVertices.reserve(pointMasses.size());
			  for (auto &pointMass : pointMasses)
				  shapeVertices.push_back(pointMass.getPosition());
			  return shapeVertices; }()),
	  pointMasses(pointMasses),
	  stiffness(0.9f),
	  damping(0.1f),
	  bounds(Shapes::BoundingBox(pointMasses)) {}

SoftBody::~SoftBody() {}

const size_t SoftBody::getPointMassCount() const
{
	return pointMasses.size();
}

const Vector2 &SoftBody::getPointMassPositionAt(const size_t index) const
{
	return pointMasses.at(index).getPosition();
}

const std::vector<Vector2> SoftBody::getPointMassPositions() const
{
	std::vector<Vector2> positions;
	positions.resize(pointMasses.size());
	for (const auto &pointMass : pointMasses)
		positions.push_back(pointMass.getPosition());
	return positions;
}

void SoftBody::setStiffness(const float newStiffness)
{
	stiffness = newStiffness;
}

void SoftBody::setDamping(const float newDamping)
{
	damping = newDamping;
}

const Shapes::BoundingBox &SoftBody::getBoundingBox() const
{
	return bounds;
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
	bounds.resize(pointMasses);

	updateCenter();
	updateRotation();
	rotate();

	netAcceleration = {0.f, 0.f};
}

void SoftBody::satisfyConstraints()
{
	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		Vector2 springForce = calculateSpringForce(getPointMassPositionAt(i),
												   getVertexPositionAt(i),
												   pointMasses.at(i).determineVelocity(),
												   stiffness,
												   damping);
		pointMasses.at(i).setPosition(Vector2Add(getPointMassPositionAt(i), springForce));
	}

	for (auto &pointMass : pointMasses)
		pointMass.satisfyConstraints();
}

void SoftBody::draw(const Color &color, const float thickness) const
{
	Shape::draw(ColorAlpha(ColorContrast(color, -0.5f), 0.4f), thickness * 0.8f);

	if (pointMasses.size() == 0)
		return;
	const std::vector<Vector2> vertices = getVertices();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		DrawLineEx(getPointMassPositionAt(i), getPointMassPositionAt((i + 1) % pointMasses.size()), thickness, color);
		DrawLineEx(getPointMassPositionAt(i), vertices.at(i), thickness, ColorAlpha(color, 0.3f));
		pointMasses.at(i).draw(color, thickness);
	}
	DrawLineEx(getPointMassPositionAt(pointMasses.size() - 1), getPointMassPositionAt(0), thickness, color);
}

void SoftBody::moveCenter(const Vector2 newPosition, const float strength)
{
	Vector2 displacement = Vector2Subtract(newPosition, center);
	for (auto &pointMass : pointMasses)
		pointMass.applyAcceleration(Vector2Scale(displacement, strength));
	center = newPosition;
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
	float averageRotationalOffset = 0.f;
	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		Vector2 centerToPointMass = Vector2Subtract(getPointMassPositionAt(i), center);
		Vector2 centerToVertex = Vector2Subtract(getVertexPositionAt(i), center);
		float angle = Vector2Angle(centerToPointMass, centerToVertex);
		averageRotationalOffset += angle;
	}
	averageRotationalOffset /= pointMasses.size();
	rotation += averageRotationalOffset;
}

Vector2 calculateSpringForce(
	const Vector2 &currentPosition,
	const Vector2 &targetPosition,
	const Vector2 &currentVelocity,
	float damping,
	float stiffness)
{
	// simple damped harmonic motion
	// F = -stiffness * displacement - damping * velocity
	Vector2 displacement = Vector2Subtract(targetPosition, currentPosition);
	Vector2 springForce = Vector2Scale(displacement, stiffness);
	Vector2 dampingForce = Vector2Scale(currentVelocity, damping);
	Vector2 totalForce = Vector2Subtract(springForce, dampingForce);

	return totalForce;
}

bool SoftBody::contains(const Vector2 &point) const
{
	// horizontal even-odd collision check
	int intersectionCount = 0;

	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		// check if point to horizontal right intersects neighboring pointMasses
		if (intersects(point,
					   getPointMassPositionAt(i),
					   getPointMassPositionAt((i + 1) % pointMasses.size())))
			intersectionCount++;
	}

	return (intersectionCount % 2) == 1;
}

bool intersects(const Vector2 &point, const Vector2 &p1, const Vector2 &p2)
{
	Vector2 lower = p1;
	Vector2 upper = p2;

	if (p1.y > p2.y)
	{
		lower = p2;
		upper = p1;
	}

	if (point.y < lower.y || point.y > upper.y || (upper.y - lower.y) == 0.f)
		return false;

	// for horizontal line against non-horizontal line
	// x-intersect = lower.x + (y-intersect - lower.y) * dx/dy
	// where y-intersect is known as point.y
	float intersectX = lower.x + (point.y - lower.y) * (upper.x - lower.x) / (upper.y - lower.y);

	// if x-intersect is to the right of point.x the lines intersect
	return intersectX > point.x;
}
