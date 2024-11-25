#pragma once

#include "World.hpp"
#include "Camera.hpp"
#include "raylib.h"
#include <vector>
#include <map>

class Editor
{
public:
	Editor(World &world, const Vector2 &topLeft);
	~Editor() = default;

	bool getIsEnabled() const { return isEnabled; }

	void update(Sidewinder::Camera &camera);
	void draw(Sidewinder::Camera &camera);

private:
	static constexpr float width = 100.f;
	static constexpr float yOffset = 30.f;
	// static constexpr float yGap = 2.f;
	static constexpr float validRadius = 20.f;

	World &world;
	Vector2 topLeft;
	bool isEnabled;
	bool isClosable;
	bool isRemovable;
	bool isPlacable;
	std::vector<Vector2> vertices;

	void createObstacle();
	bool isSelfIntersecting(Vector2 &position);
	bool isValidPosition(Vector2 &position);
};
