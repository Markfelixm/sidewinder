#include "World.hpp"

#include "Player.hpp"
#include "Obstacle.hpp"
#include <fstream>

World::World(Input &input) : input(input), gravity(0.f), playerFeelsGravity(true)
{
	entities.emplace_back(std::make_unique<Player>(input, Vector2{800.f, 500.f}, 10));

	std::vector<Vector2>
		floorPoints = {{0.f, 800.f}, {3200.f, 800.f}, {3200.f, 1300.f}, {0.f, 1300.f}};
	std::unique_ptr<Obstacle> floor = std::make_unique<Obstacle>(floorPoints, 1000.f);
	floor->setColor(BLACK);
	floor->setStiffness(0.8f);
	floor->setIsStationary(true);
	entities.push_back(std::move(floor));

	std::vector<Vector2>
		ceilingPoints = {{0.f, -800.f}, {3200.f, -800.f}, {3200.f, -1300.f}, {0.f, -1300.f}};
	std::unique_ptr<Obstacle> ceiling = std::make_unique<Obstacle>(ceilingPoints, 1000.f);
	ceiling->setColor(BLACK);
	ceiling->setStiffness(0.8f);
	ceiling->setIsStationary(true);
	entities.push_back(std::move(ceiling));
}

World::~World()
{
	std::ofstream outFile("vertices.txt");

	if (!outFile)
		return;

	for (auto &entity : entities)
	{
		for (auto &vertex : entity->getVertices())
			outFile << "{" << vertex.x << ", " << vertex.y << "}," << std::endl;
		outFile << std::endl;
	}
	outFile.close();
}

std::vector<std::unique_ptr<SoftBody>> &World::getEntities()
{
	return entities;
}

void World::update(const float deltaTime)
{
	for (auto &entity : entities)
	{
		if (!(dynamic_cast<Player *>(entity.get()) && !playerFeelsGravity))
			entity->applyAcceleration({0.f, gravity});

		entity->update(deltaTime);

		for (size_t i = 1; i < entities.size(); i++)
			entity->handleSoftBodyCollision(*entities.at(i).get());
		entity->satisfyConstraints();
	}
}
