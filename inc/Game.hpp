#pragma once

#include "Scene.hpp"

#include "raylib.h"

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
	~Game();

	void draw();
	void update(float deltaTime);

private:
	ViewState viewState;
	// Title title;
	Scene scene;
	// Retry retry;
};
