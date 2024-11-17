#include "Scene.hpp"

Scene::Scene(Input &input) : world(World(input, 3200.f, 2000.f)), camera(Sidewinder::Camera(world)) {}

void Scene::update(const float deltaTime)
{
	world.update(deltaTime);
	// TODO: move camera position with spring attached to target, target is anchor/fixed
	std::shared_ptr<SoftBody> &target = world.getEntities().at(0);
	camera.setCameraCenterInWorld(target->getCenterPosition());
}

void Scene::draw()
{
	for (auto &entity : world.getEntities())
	{
		// if (camera.getBoundingBox().intersects(entity->getBoundingBox()))
		entity->draw(camera);
	}
}
