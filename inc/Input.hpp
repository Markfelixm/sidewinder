#pragma once

#include "raylib.h"

#include <map>

struct Input
{
	enum binds
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		JUMP,
	};

	std::map<binds, bool> isDown;

	void update()
	{
		isDown[UP] = IsKeyDown(KEY_W);
		isDown[UP] = IsKeyDown(KEY_UP);
		isDown[DOWN] = IsKeyDown(KEY_S);
		isDown[DOWN] = IsKeyDown(KEY_DOWN);
		isDown[LEFT] = IsKeyDown(KEY_A);
		isDown[LEFT] = IsKeyDown(KEY_LEFT);
		isDown[RIGHT] = IsKeyDown(KEY_D);
		isDown[RIGHT] = IsKeyDown(KEY_RIGHT);
		isDown[JUMP] = IsKeyDown(KEY_SPACE);
	}
};
