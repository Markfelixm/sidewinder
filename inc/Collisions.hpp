#pragma once

#include "PointMass.hpp"
#include "Shape.hpp"
#include <vector>

bool collides(Shape &attacker, Shape &defender);
bool contains(Vector2 &position, std::vector<PointMass> &container);
