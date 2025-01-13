#pragma once

// #include "Input.hpp"
// #include "Scene.hpp"
#include "raylib.h"
#include "Framed.hpp"
#include "Shape.hpp"
#include "Animated.hpp"
#include <memory>

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
	// Input input;
	// ViewState viewState;
	// Title title;
	// Scene scene;
	// Retry retry;

	std::unique_ptr<Framed> attacker;
	std::unique_ptr<Framed> defender;
	std::unique_ptr<Animated> animated;
};
