#pragma once

#include "raylib.h"

class Point
{
public:
	Point(const Vector2 &initialPosition);
	~Point();

	void setPosition(const Vector2 &newPosition);
	const Vector2 &getPosition() const;
	void applyAcceleration(const Vector2 &acceleration);
	const Vector2 &getAcceleration() const;

	virtual void update(const float deltaTime);
	virtual void draw(const Color &color, const float radius) const;

protected:
	Vector2 position;
	Vector2 previousPosition;
	Vector2 netAcceleration;

private:
	Point() = delete;
};
