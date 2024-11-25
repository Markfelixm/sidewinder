#include "World.hpp"

#include "Player.hpp"
#include "Obstacle.hpp"

World::World(Input &input) : input(input)
{
	entities.emplace_back(std::make_unique<Player>(input, Vector2{800.f, 500.f}, 12));

	std::vector<Vector2>
		trianglePoints = {{800.f, 100.f}, {850.f, 200.f}, {950.f, 200.f}};
	entities.push_back(std::make_unique<Obstacle>(trianglePoints, 100.f));

	std::vector<Vector2>
		squarePoints = {{0.f, 0.f}, {400.f, 0.f}, {400.f, 250.f}, {0.f, 250.f}};
	entities.push_back(std::make_unique<Obstacle>(squarePoints, 10.f));
	entities.at(2)->setColor(RED);
	entities.at(2)->setStiffness(0.1f);
	entities.at(2)->setDamping(0.01f);
}

std::vector<std::unique_ptr<SoftBody>> &World::getEntities()
{
	return entities;
}

void World::update(const float deltaTime)
{
	static const Vector2 gravity = {0.f, 0.f};
	// TODO: let softbodies be stationary/fixed/static

	for (auto &entity : entities)
	{
		// 	if (auto player = std::dynamic_pointer_cast<Player>(entity))
		// {
		// handle player
		// }
		entity->applyAcceleration(gravity);
		entity->update(deltaTime);
		for (auto &other : entities)
			entity->handleSoftBodyCollision(*other.get());
		entity->satisfyConstraints();
	}
}
