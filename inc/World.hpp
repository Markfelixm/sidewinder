#pragma once

#include "Input.hpp"
#include "Player.hpp"
#include "SoftBody.hpp"
#include "raylib.h"
#include <vector>
#include <memory>

class World
{
public:
	World() = delete;
	World(Input &input);
	~World();

	Player &getPlayer() { return player; }
	float *getPGravity() { return &gravity; }
	bool *getPPlayerFeelsGravity() { return &playerFeelsGravity; }
	std::vector<std::unique_ptr<SoftBody>> &getEntities() { return entities; }
	void addEntity(std::unique_ptr<SoftBody> pEntity) { entities.push_back(std::move(pEntity)); }

	void update(const float deltaTime);

private:
	std::vector<std::unique_ptr<SoftBody>> entities;
	Input &input;
	Player player;

	float gravity;
	bool playerFeelsGravity;
};
