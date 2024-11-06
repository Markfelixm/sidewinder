#pragma once

#include "World.hpp"
#include "Camera.hpp"

class Scene
{
public:
	Scene();
	~Scene();

	void update(const float deltaTime);
	void draw();

private:
	World world;
	Sidewinder::Camera camera;
};
