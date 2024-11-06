#pragma once

#include "Entity.hpp"

#include "raylib.h"

#include <vector>

class World
{
public:
	World();
	~World();

	std::vector<std::shared_ptr<Entity>> &getEntities();

	void update(const float deltaTime);

private:
	std::vector<std::shared_ptr<Entity>> entities;
};
