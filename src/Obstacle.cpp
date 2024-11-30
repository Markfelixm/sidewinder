#include "Obstacle.hpp"
#include "PointMass.hpp"

Obstacle::Obstacle(const std::vector<Vector2> &vertices, const float mass) : SoftBody([vertices, mass]()
																					  {
	std::vector<PointMass> pointMasses;
	pointMasses.reserve(vertices.size());
	for (auto &vertex : vertices)
		pointMasses.push_back(PointMass(vertex, mass));
	return pointMasses; }())
{
	SoftBody::setColor(BLUE);
}

void Obstacle::applyAcceleration(const Vector2 &acceleration)
{
	SoftBody::applyAcceleration(acceleration);
}

void Obstacle::update(const float deltaTime)
{
	SoftBody::update(deltaTime);
}

void Obstacle::satisfyConstraints()
{
	SoftBody::satisfyConstraints();
}

void Obstacle::draw(const Sidewinder::Camera &camera) const
{
	SoftBody::draw(camera);
}
