#include <math.h>

#include "Car.hpp"
#include "raymath.h"

Car::Car(const Vector2 &initialPosition) : position(initialPosition), rotation(0.0f), speed(0.0f), acceleration(150.0f), brakingAcceleration(-400.0f), idleAcceleration(-40.0f)
{
	texture = LoadTexture("res/car.png");
}

Car::~Car()
{
	UnloadTexture(texture);
}

void Car::draw() const
{

	static const Rectangle sourceRec = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
	Rectangle destRec = {position.x, position.y, (float)texture.width, (float)texture.height};
	static const Vector2 origin = {texture.width / 2.0f, texture.height / 2.0f};
	DrawTexturePro(texture, sourceRec, destRec, origin, rotation, WHITE);
}

void Car::update(float dt)
{
	if (IsKeyDown(KEY_LEFT))
		rotation -= 4.0f;
	if (IsKeyDown(KEY_RIGHT))
		rotation += 4.0f;

	if (IsKeyDown(KEY_UP))
		speed += acceleration * dt;
	if (IsKeyDown(KEY_DOWN))
		speed += brakingAcceleration * dt;

	if (speed > 0.0f)
		speed += idleAcceleration * dt;
	else
		speed = 0.0f;

	position.x += sin(rotation * DEG2RAD) * speed * dt;
	position.y -= cos(rotation * DEG2RAD) * speed * dt;
}

const Vector2 &Car::getPosition() const
{
	return position;
}
