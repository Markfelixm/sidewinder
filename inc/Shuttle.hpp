#pragma once

#include "Vector.hpp"
#include "Framed.hpp"
#include "Input.hpp"

struct Shuttle
{
	Input &input;
	Framed shuttle;

	Shuttle(Input &input, const V2 &position, float rotation) : input(input), shuttle(Framed(createShuttlePointMasses(position, rotation), 1000.f)) {}

	std::vector<PointMass> createShuttlePointMasses(const V2 &position, float rotation)
	{
		std::vector<PointMass> pointMasses;
		pointMasses.reserve(5);
		pointMasses.emplace_back(PointMass(position + V2(-30.f, -30.f), 30.f));
		pointMasses.emplace_back(PointMass(position + V2(0.f, -50.f), 30.f));
		pointMasses.emplace_back(PointMass(position + V2(30.f, -30.f), 30.f));
		pointMasses.emplace_back(PointMass(position + V2(30.f, 30.f), 30.f));
		pointMasses.emplace_back(PointMass(position + V2(-30.f, 30.f), 30.f));

		return pointMasses;
	}

	void update(float deltaTime)
	{
		V2 direction = shuttle.frame.points.at(1).position - shuttle.actual.getCenter();
		direction = direction.normalize();

		if (input.isDown[Input::binds::UP])
			shuttle.actual.acceleration += direction * 800.f;
		if (input.isDown[Input::binds::DOWN])
			shuttle.actual.acceleration -= direction * 800.f;
		if (input.isDown[Input::binds::LEFT])
			shuttle.actual.rotate(-5.f);
		if (input.isDown[Input::binds::RIGHT])
			shuttle.actual.rotate(5.f);

		shuttle.update(deltaTime);
	}

	void draw(Viewport &viewport, Color color) const
	{
		shuttle.draw(viewport, color);
	}
};
