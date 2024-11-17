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
		isDown[UP] = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
		isDown[DOWN] = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
		isDown[LEFT] = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
		isDown[RIGHT] = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
		isDown[JUMP] = IsKeyDown(KEY_SPACE);
	}
};
