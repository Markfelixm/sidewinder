#pragma once

#include "World.hpp"
#include "Camera.hpp"
#include "raylib.h"
#include <vector>
#include <map>

struct Creator
{
	std::vector<Vector2> vertices;
	float mass;
	float stiffness;
	float damping;
	bool isStationary;
	Color color;
	bool isClosable;
	bool isRemovable;
	bool isPlacable;
};

class Editor
{
public:
	Editor(World &world);
	~Editor() = default;

	bool getIsEnabled() const { return isEnabled; }

	void update(Sidewinder::Camera &camera);
	void draw(Sidewinder::Camera &camera);

private:
	World &world;

	static constexpr float validRadius = 20.f;
	bool isEnabled;

	Creator creator;

	void createObstacle();
	bool isSelfIntersecting(Vector2 &position);
	bool isValidPosition(Vector2 &position);
};
