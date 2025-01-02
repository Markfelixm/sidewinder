#include "Scene.hpp"

Scene::Scene(Input &input) : world(World(input)), camera(Sidewinder::Camera(world)), editor(Editor(world)) {}

void Scene::update(const float deltaTime)
{
	if (editor.getIsEnabled())
		editor.update(camera);
	else
	{
		world.update(deltaTime);
		// TODO: move camera position with spring attached to target, target is anchor/fixed
		camera.setCameraCenterInWorld(world.getPlayer().getCenterPosition());
	}
}

void Scene::draw()
{
	for (auto &entity : world.getEntities())
	{
		if (camera.getBoundingBox().intersects(entity->getBoundingBox()))
			entity->draw(camera);
	}
	world.getPlayer().draw(camera);
	editor.draw(camera);
}
