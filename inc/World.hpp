#pragma once

#include "SoftBody.hpp"

#include "raylib.h"

#include <vector>

class World
{
public:
	World(const float width, const float height);
	~World();

	std::vector<std::shared_ptr<SoftBody>> &getEntities();
	const float getWidth() const;
	const float getHeight() const;

	void update(const float deltaTime);

private:
	World() = delete;
	std::vector<std::shared_ptr<SoftBody>> entities;

	const float width;
	const float height;
};
