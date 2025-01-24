#pragma once

#include "Input.hpp"
#include "raylib.h"
#include "Viewport.hpp"
#include "Planet.hpp"
#include "Shuttle.hpp"
#include "Animated.hpp"
#include <memory>
#include <vector>

class Game
{
	enum ViewState
	{
		TITLE,
		SCENE,
		RETRY
	};

public:
	Game();
	~Game() = default;

	void draw();
	void update(float deltaTime);

private:
	Input input;
	Viewport viewport;

	std::unique_ptr<Planet> planet;
	std::unique_ptr<Planet> moon;
	std::unique_ptr<Animated> orbit;
	std::unique_ptr<Shuttle> shuttle;
};
