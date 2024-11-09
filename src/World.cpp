#include "World.hpp"

#include "PointMass.hpp"

World::World(const float width, const float height) : width(width), height(height)
{
	std::vector<PointMass> trianglePoints = {PointMass({100.f, 100.f}, 100.f), PointMass({50.f, 200.f}, 100.f), PointMass({150.f, 200.f}, 100.f)};
	std::shared_ptr<SoftBody> triangle = std::make_shared<SoftBody>(trianglePoints);
	triangle->setColor(BLUE);
	entities.push_back(triangle);
	std::vector<PointMass> pentagonPoints = {PointMass({600.f, 600.f}, 100.f), PointMass({550.f, 700.f}, 100.f), PointMass({575.f, 800.f}, 100.f), PointMass({625.f, 800.f}, 100.f), PointMass({650.f, 700.f}, 100.f)};
	std::shared_ptr<SoftBody> pentagon = std::make_shared<SoftBody>(pentagonPoints);
	pentagon->setColor(RED);
	entities.push_back(pentagon);

	std::vector<PointMass> topleftPoints = {PointMass({0.f, 0.f}, 100.f), PointMass({width / 2.f, 0.f}, 100.f), PointMass({width / 2.f, height / 2.f}, 100.f), PointMass({0.f, height / 2.f}, 100.f)};
	std::shared_ptr<SoftBody> topleft = std::make_shared<SoftBody>(topleftPoints);
	topleft->setColor(BLACK);
	entities.push_back(topleft);
	std::vector<PointMass> bottomRightPoints = {PointMass({width / 2.f, height / 2.f}, 100.f), PointMass({width, height / 2.f}, 100.f), PointMass({width, height}, 100.f), PointMass({width / 2.f, height}, 100.f)};
	std::shared_ptr<SoftBody> bottomRight = std::make_shared<SoftBody>(bottomRightPoints);
	bottomRight->setColor(YELLOW);
	entities.push_back(bottomRight);
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
	static const Vector2 gravity = {0.f, 20.f};

	for (auto &entity : entities)
	{
		entity->applyAcceleration(gravity);
		entity->update(deltaTime);
	}
}
