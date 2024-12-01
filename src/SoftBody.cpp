#include "SoftBody.hpp"
#include "Camera.hpp"

#include "raymath.h"

#include <cmath>
#include <limits>

// TODO: find bug coupling damping, gravity, and velocity. probably related to frame and springs
// TODO: abstract physics and collision utilities

const bool isHorizontalIntersect(const Vector2 &point, const Vector2 &a, const Vector2 &b);
const float projectionRatio(const Vector2 &point, const Vector2 &a, const Vector2 &b);
const float distanceToLine(const Vector2 &point, const Vector2 &a, const Vector2 &b, const bool useOptimization);
Vector2 calculateSpringForce(
	const Vector2 &currentPosition,
	const Vector2 &targetPosition,
	const Vector2 &currentVelocity,
	float stiffness,
	float damping);

SoftBody::SoftBody(const std::vector<PointMass> &pointMasses)
	: Shape([&pointMasses]()
			{
			  std::vector<Vector2> shapeVertices;
			  shapeVertices.reserve(pointMasses.size());
			  for (auto &pointMass : pointMasses)
				  shapeVertices.push_back(pointMass.getPosition());
			  return shapeVertices; }()),
	  pointMasses(pointMasses),
	  stiffness(0.1f),
	  damping(0.05f),
	  isStationary(false),
	  bounds(Sidewinder::BoundingBox(getPointMassPositions()))
{
}

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

void SoftBody::setColor(const Color &newColor)
{
	color = newColor;
}

void SoftBody::setIsStationary(bool state)
{
	isStationary = state;
}

const Sidewinder::BoundingBox &SoftBody::getBoundingBox() const
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
	if (isStationary)
		return;
	for (auto &pointMass : pointMasses)
		pointMass.update(deltaTime);
	bounds.resize(getPointMassPositions());

	updateCenter();
	updateRotation();
	updatePolars();

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
		springForce = Vector2Scale(springForce, 0.5);
		pointMasses.at(i).setPosition(Vector2Add(getPointMassPositionAt(i), springForce));
	}

	for (auto &pointMass : pointMasses)
		pointMass.satisfyConstraints();
}

void SoftBody::draw(const Sidewinder::Camera &camera) const
{
	// TODO: determine thickess from camera
	const float thickness = 5.f;

	if (pointMasses.size() == 0)
		return;
	const std::vector<Vector2> vertices = getVertices();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		// SoftBody
		// DrawLineEx(getPointMassPositionAt(i), getPointMassPositionAt((i + 1) % pointMasses.size()), thickness, color);
		// // Shape
		// DrawLineEx(vertices.at(i), vertices.at((i + 1) % pointMasses.size()), thickness * 0.8f, ColorAlpha(ColorContrast(color, -0.5f), 0.4f));
		// // spring
		// DrawLineEx(getPointMassPositionAt(i), vertices.at(i), thickness, ColorAlpha(color, 0.3f));
		// PointMass
		// DrawCircleV(getPointMassPositionAt(i), thickness, color);
		DrawLineEx(camera.worldToScreen(getPointMassPositionAt(i)), camera.worldToScreen(getPointMassPositionAt((i + 1) % pointMasses.size())), thickness, color);
		DrawCircleV(camera.worldToScreen(getPointMassPositionAt(i)), thickness * 0.67f, color);
	}
}

// TODO: rm?
void SoftBody::moveCenter(const Vector2 newPosition, const float strength)
{
	Vector2 displacement = Vector2Subtract(newPosition, center);
	for (auto &pointMass : pointMasses)
		pointMass.applyAcceleration(Vector2Scale(displacement, strength));
	center = newPosition;
}

void SoftBody::handleSoftBodyCollision(SoftBody &other)
{
	if (!bounds.intersects(other.getBoundingBox()))
		return;
	for (auto &pointMass : pointMasses)
	{
		if (other.contains(pointMass.getPosition()))
			other.resolveCollision(pointMass);
	}
}

bool SoftBody::contains(const Vector2 &point) const
{
	// horizontal even-odd collision check
	int intersectionCount = 0;

	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		// check if point to horizontal right intersects neighboring pointMasses
		if (isHorizontalIntersect(point,
								  getPointMassPositionAt(i),
								  getPointMassPositionAt((i + 1) % pointMasses.size())))
			intersectionCount++;
	}

	return (intersectionCount % 2) == 1;
}

void SoftBody::resolveCollision(PointMass &collider)
{
	// TODO: finding nearest edge does not play nice with shapes of greatly varying segment lengths
	// determine direction and magnitude of collision
	std::pair<PointMass &, PointMass &> edge = findNearestEdge(collider.getPosition());

	const float collisionRatio = projectionRatio(collider.getPosition(),
												 edge.first.getPosition(),
												 edge.second.getPosition());
	Vector2 edgeLine = Vector2Subtract(edge.second.getPosition(), edge.first.getPosition());
	Vector2 collisionPoint = Vector2Add(edge.first.getPosition(), Vector2Scale(edgeLine, collisionRatio));

	const Vector2 collisionNormal = Vector2Normalize(Vector2Subtract(collisionPoint, collider.getPosition()));
	const float penetrationDepth = Vector2Distance(collider.getPosition(), collisionPoint);

	// resolve collider and edge proportional to mass
	const float edgeMass = edge.first.getMass() + edge.second.getMass();
	const float totalMass = collider.getMass() + edgeMass;
	const float colliderResolutionDistance = penetrationDepth * edgeMass / totalMass;
	const float edgeResolutionDistance = penetrationDepth - colliderResolutionDistance;
	// also take into account the closeness of the collision point to either end of the edge
	const float positionOffset = collisionRatio * 2.f - 1.f;

	// resolve positions
	edge.first.setPosition(Vector2Add(edge.first.getPosition(), Vector2Scale(collisionNormal, -1.f * edgeResolutionDistance)));
	edge.second.setPosition(Vector2Add(edge.second.getPosition(), Vector2Scale(collisionNormal, -1.f * edgeResolutionDistance)));
	collider.setPosition(Vector2Add(collider.getPosition(), Vector2Scale(collisionNormal, colliderResolutionDistance)));
}

std::pair<PointMass &, PointMass &> SoftBody::findNearestEdge(const Vector2 &point)
{
	float minDistance = std::numeric_limits<float>::max();
	size_t nearestIndex = 0;

	for (size_t i = 0; i < pointMasses.size(); i++)
	{
		float distance = distanceToLine(point,
										getPointMassPositionAt(i),
										getPointMassPositionAt((i + 1) % pointMasses.size()),
										true);

		if (distance < minDistance)
		{
			minDistance = distance;
			nearestIndex = i;
		}
	}

	return std::pair<PointMass &, PointMass &>(pointMasses.at(nearestIndex),
											   pointMasses.at((nearestIndex + 1) % pointMasses.size()));
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
	rotation = fmodf(rotation, 2.f * PI);
}

const bool isHorizontalIntersect(const Vector2 &point, const Vector2 &a, const Vector2 &b)
{
	Vector2 lower = a;
	Vector2 upper = b;

	if (a.y > b.y)
	{
		lower = b;
		upper = a;
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

const float distanceToLine(const Vector2 &point, const Vector2 &a, const Vector2 &b, const bool useOptimization)
{
	// equation described by sebastian lague https://www.youtube.com/watch?v=KHuI9bXZS74
	float numerator = fabsf((point.x - a.x) * (-1.f * b.y + a.y) + (point.y - a.y) * (b.x - a.x));
	if (useOptimization)
		return numerator;

	float denominator = sqrtf((-b.y + a.y) * (-b.y + a.y) + (b.x - a.x) * (b.x - a.x));
	return fabsf(numerator / denominator);
}

const float projectionRatio(const Vector2 &point, const Vector2 &a, const Vector2 &b)
{
	// if point were to be projected onto line AB, the ratio is its closeness to A
	Vector2 aToB = Vector2Subtract(b, a);
	Vector2 aToPoint = Vector2Subtract(point, a);

	float t = Vector2DotProduct(aToPoint, aToB) / Vector2LengthSqr(aToB);
	return std::fmax(0.f, std::fmin(1.0f, t));
}

Vector2 calculateSpringForce(
	const Vector2 &currentPosition,
	const Vector2 &targetPosition,
	const Vector2 &currentVelocity,
	float stiffness,
	float damping)
{
	// simple damped harmonic motion
	// F = -stiffness * displacement - damping * velocity
	Vector2 displacement = Vector2Subtract(targetPosition, currentPosition);
	Vector2 springForce = Vector2Scale(displacement, stiffness);
	Vector2 dampingForce = Vector2Scale(currentVelocity, damping);
	Vector2 totalForce = Vector2Subtract(springForce, dampingForce);

	return totalForce;
}
