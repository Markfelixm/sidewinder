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

	std::vector<std::unique_ptr<SoftBody>> &getEntities();
	void addEntity(std::unique_ptr<SoftBody> pEntity) { entities.push_back(std::move(pEntity)); }

	void update(const float deltaTime);

private:
	std::vector<std::unique_ptr<SoftBody>> entities;
	Input &input;
};
