#include "World.hpp"

#include "Player.hpp"
#include "Obstacle.hpp"

World::World(Input &input) : input(input), gravity(500.f)
{
	entities.emplace_back(std::make_unique<Player>(input, Vector2{800.f, 500.f}, 10));

	std::vector<Vector2>
		leftPoints = {{100.f, 100.f}, {200.f, 100.f}, {200.f, 200.f}, {100.f, 200.f}};
	entities.push_back(std::make_unique<Obstacle>(leftPoints, 200.f));
	entities.at(1)->setColor((Color){0, 0, 242, 255});
	entities.at(1)->setStiffness(0.1f);

	std::vector<Vector2>
		rightPoints = {{300.f, 100.f}, {400.f, 100.f}, {400.f, 300.f}, {300.f, 300.f}};
	entities.push_back(std::make_unique<Obstacle>(rightPoints, 200.f));
	entities.at(2)->setColor((Color){242, 0, 0, 255});
	entities.at(2)->setStiffness(0.3f);

	std::vector<Vector2>
		floorPoints = {{0.f, 800.f}, {2400.f, 800.f}, {2400.f, 1100.f}, {0.f, 1100.f}};
	entities.push_back(std::make_unique<Obstacle>(floorPoints, 1000.f));
	entities.at(3)->setColor(BLACK);
	entities.at(3)->setStiffness(0.8f);
	entities.at(3)->setIsStationary(true);
}

std::vector<std::unique_ptr<SoftBody>> &World::getEntities()
{
	return entities;
}

void World::update(const float deltaTime)
{
	for (auto &entity : entities)
	{
		// if (auto player = std::dynamic_pointer_cast<Player>(entity))
		// {
		// 	// handle player
		// }
		entity->applyAcceleration({0.f, gravity});
		entity->update(deltaTime);
		for (auto &other : entities)
			entity->handleSoftBodyCollision(*other.get());
		entity->satisfyConstraints();
	}
}
