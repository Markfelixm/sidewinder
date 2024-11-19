#include "World.hpp"

#include "Player.hpp"
#include "Obstacle.hpp"

World::World(Input &input, const float width, const float height) : input(input), width(width), height(height)
{
	entities.emplace_back(std::make_shared<Player>(input, Vector2{100.f, 400.f}, 6));

	std::vector<Vector2>
		trianglePoints = {{100.f, 100.f}, {50.f, 200.f}, {150.f, 200.f}};
	entities.push_back(std::make_shared<Obstacle>(trianglePoints, 100.f));

	std::vector<Vector2>
		squarePoints = {{300.f, 300.f}, {300.f, 600.f}, {600.f, 600.f}, {600.f, 300.f}};
	entities.push_back(std::make_shared<Obstacle>(squarePoints, 10.f));
	entities.at(2)->setColor(RED);
	entities.at(2)->setStiffness(0.1f);
	entities.at(2)->setDamping(0.01f);
}

World::~World() {}

std::vector<std::shared_ptr<SoftBody>> &World::getEntities()
{
	return entities;
}

const float World::getWidth() const
{
	return width;
}

const float World::getHeight() const
{
	return height;
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
