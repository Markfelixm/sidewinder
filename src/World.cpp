#include "World.hpp"

World::World()
{
}

World::~World() {}

std::vector<std::shared_ptr<Entity>> &World::getEntities()
{
	return entities;
}

void World::update(const float deltaTime)
{
}
