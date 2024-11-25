#include "Scene.hpp"

Scene::Scene(Input &input) : world(World(input)), camera(Sidewinder::Camera(world)), editor(Editor(world, {20.f, 20.f})) {}

void Scene::update(const float deltaTime)
{
	if (editor.getIsEnabled())
		editor.update(camera);
	else
		world.update(deltaTime);
	// TODO: move camera position with spring attached to target, target is anchor/fixed
	std::shared_ptr<SoftBody> &target = world.getEntities().at(0);
	camera.setCameraCenterInWorld(target->getCenterPosition());
}

void Scene::draw()
{
	editor.draw(camera);
	DrawCircleLinesV(camera.getCameraCenterInWorld(), 5.f, BLACK);
	for (auto &entity : world.getEntities())
	{
		// if (camera.getBoundingBox().intersects(entity->getBoundingBox()))
		entity->draw(camera);
	}
}
