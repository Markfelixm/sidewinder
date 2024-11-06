#include "Scene.hpp"

Scene::Scene() : world(World()), camera(Sidewinder::Camera({{0.f, 0.f}, {1600.f, 1000.f}})) {}

Scene::~Scene() {}

void Scene::update(const float deltaTime)
{
	world.update(deltaTime);
	camera.update(deltaTime);
}

void Scene::draw()
{
	for (auto &entity : world.getEntities())
	{
		if (camera.getBoundingBox().intersects(entity->getBoundingBox()))
			entity->draw();
	}
}
