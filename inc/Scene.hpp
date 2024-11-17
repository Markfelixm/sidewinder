#pragma once

#include "World.hpp"
#include "Camera.hpp"

class Input;

class Scene
{
public:
	// TODO: pick a constructor style and apply everywhere
	Scene() = delete;
	Scene(Input &input);
	~Scene() = default;

	void update(const float deltaTime);
	void draw();

private:
	World world;
	Sidewinder::Camera camera;
};
