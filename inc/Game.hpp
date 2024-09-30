#pragma once

#include "raylib.h"

#include "Track.hpp"
#include "Car.hpp"

class Game
{
public:
	Game();
	~Game();

	void draw() const;
	void update(float dt);

private:
	Track track;
	Car car;
};
