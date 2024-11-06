#pragma once

#include "BoundingBox.hpp"

class Entity
{
public:
	virtual ~Entity() = default;

	virtual const Sidewinder::BoundingBox &getBoundingBox() const;

	virtual void update(const float deltaTime) = 0;
	virtual void draw() const;
};
