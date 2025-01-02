#include "World.hpp"

#include "Obstacle.hpp"
#include "BoundingBox.hpp"
#include <fstream>

World::World(Input &input) : input(input), player(Player(input, Vector2{800.f, 500.f}, 10)), gravity(0.f), playerFeelsGravity(true)
{
	std::vector<Vector2>
		floorVertices = {{0.f, 800.f}, {3200.f, 800.f}, {3200.f, 1300.f}, {0.f, 1300.f}};
	std::unique_ptr<Obstacle> floor = std::make_unique<Obstacle>(floorVertices, 1000.f);
	floor->setColor(BLACK);
	floor->setStiffness(0.8f);
	floor->setIsStationary(true);
	entities.push_back(std::move(floor));

	std::vector<Vector2>
		ceilingVertices = {{0.f, -800.f}, {3200.f, -800.f}, {3200.f, -1300.f}, {0.f, -1300.f}};
	std::unique_ptr<Obstacle> ceiling = std::make_unique<Obstacle>(ceilingVertices, 1000.f);
	ceiling->setColor(BLACK);
	ceiling->setStiffness(0.8f);
	ceiling->setIsStationary(false);
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

void World::update(const float deltaTime)
{

	for (auto &entity : entities)
	{
		entity->applyAcceleration({0.f, gravity});
		entity->update(deltaTime);
	}
	if (playerFeelsGravity)
		player.applyAcceleration({0.f, gravity});
	player.update(deltaTime);

	for (size_t i = 0; i < entities.size(); i++)
	{
		for (size_t j = 1; j < entities.size(); j++)
			entities.at(i)->handleSoftBodyCollision(*entities.at(j));
		player.handleSoftBodyCollision(*entities.at(i));
		entities.at(i)->satisfyConstraints();
	}
	player.satisfyConstraints();
}
