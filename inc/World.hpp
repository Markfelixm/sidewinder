#pragma once

#include "SoftBody.hpp"
#include "raylib.h"
#include <vector>
#include <memory>

class Input;

class World
{
public:
	World() = delete;
	World(Input &input);
	~World() = default;

	// TODO: unique ptr
	std::vector<std::shared_ptr<SoftBody>> &getEntities();
	void addEntity(std::shared_ptr<SoftBody> pEntity) { entities.push_back(pEntity); }

	void update(const float deltaTime);

private:
	std::vector<std::shared_ptr<SoftBody>> entities;
	Input &input;
};
