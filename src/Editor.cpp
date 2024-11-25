#include "Editor.hpp"
#include "Obstacle.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
#include <memory>

// TODO: move to utility
bool circleContains(Vector2 &circlePositon, float circleRadius, Vector2 &position);
bool isClockwise(const Vector2 &a, const Vector2 &b, const Vector2 &position);
bool segmentsIntersect(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Vector2 &d);

Editor::Editor(World &world, const Vector2 &topLeft) : world(world), topLeft(topLeft), isEnabled(false), isClosable(false), isRemovable(false), isPlacable(true)
{
}

void Editor::update(Sidewinder::Camera &camera)
{
	Vector2 mouseInWorld = camera.screenToWorld(GetMousePosition());

	isClosable = (vertices.size() > 2 && circleContains(vertices.front(), validRadius, mouseInWorld));
	// TODO: fix: cant close when last segment would make concave shape
	// isClosable = isClosable && !isSelfIntersecting(vertices.front());
	isRemovable = (!vertices.empty() && circleContains(vertices.back(), validRadius, mouseInWorld));
	isPlacable = isValidPosition(mouseInWorld);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (isClosable)
		{
			createObstacle();
			vertices.clear();
		}
		else if (isRemovable)
		{
			vertices.pop_back();
			isRemovable = false;
		}
		else if (isPlacable)
			vertices.push_back(mouseInWorld);
	}
}

void Editor::draw(Sidewinder::Camera &camera)
{
	if (!isEnabled)
	{
		if (GuiButton((Rectangle){topLeft.x, topLeft.y, topLeft.x + width, topLeft.y + yOffset}, "Enable Editor"))
			isEnabled = true;
		return;
	}
	if (GuiButton((Rectangle){topLeft.x, topLeft.y, topLeft.x + width, topLeft.y + yOffset}, "Disable Editor"))
	{
		vertices.clear();
		isEnabled = false;
		return;
	}
	if (!vertices.empty())
	{
		if (isClosable)
			DrawCircleV(camera.worldToScreen(vertices.front()), validRadius, GREEN);
		DrawCircleLinesV(camera.worldToScreen(vertices.front()), validRadius, BLACK);
		DrawCircleV(camera.worldToScreen(vertices.front()), 4.f, BLACK);
		if (isRemovable)
			DrawCircleV(camera.worldToScreen(vertices.back()), validRadius, BLACK);
		if (!isPlacable && !isClosable && !isRemovable)
			DrawLineEx(camera.worldToScreen(vertices.back()), GetMousePosition(), 8.f, RED);
		if (isClosable) // snap to closable
			DrawLineV(camera.worldToScreen(vertices.back()), camera.worldToScreen(vertices.front()), BLACK);
		else
		{
			DrawCircleV(GetMousePosition(), 4.f, BLACK);
			DrawLineV(camera.worldToScreen(vertices.back()), GetMousePosition(), BLACK);
		}
	}
	for (size_t i = 1; i < vertices.size(); i++)
	{
		DrawCircleV(camera.worldToScreen(vertices.at(i)), 4.f, BLACK);
		DrawCircleLinesV(camera.worldToScreen(vertices.at(i)), validRadius, BLACK);
		DrawLineV(camera.worldToScreen(vertices.at(i - 1)), camera.worldToScreen(vertices.at(i)), BLACK);
	}
}

void Editor::createObstacle()
{
	world.addEntity(std::make_shared<Obstacle>(vertices, 10.f));
}

bool Editor::isSelfIntersecting(Vector2 &position)
{
	for (size_t i = 1; vertices.size() > 2 && i < (vertices.size() - 1); i++)
	{
		if (segmentsIntersect(
				vertices.at(i - 1),
				vertices.at(i),
				vertices.back(),
				position))
			return true;
	}
	return false;
}

bool Editor::isValidPosition(Vector2 &position)
{
	for (auto &vertex : vertices)
	{
		if (circleContains(vertex, validRadius, position))
			return false;
	}
	// TODO: add minimum angle between lines threshold to avoid colinears
	// TODO: check against existing entities, check bounding box first
	return !isSelfIntersecting(position);
}

bool circleContains(Vector2 &circlePositon, float circleRadius, Vector2 &position)
{
	float distance = Vector2LengthSqr(Vector2Subtract(circlePositon, position));
	return distance < (circleRadius * circleRadius);
}

bool isClockwise(const Vector2 &a, const Vector2 &b, const Vector2 &position)
{
	return (b.y - a.y) * (position.x - b.x) - (b.x - a.x) * (position.y - b.y) > 0;
}

bool segmentsIntersect(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Vector2 &d)
{
	// check if endpoints of one segment are on opposite sides of other segment
	bool abc = isClockwise(a, b, c);
	bool abd = isClockwise(a, b, d);
	bool cda = isClockwise(c, d, a);
	bool cdb = isClockwise(c, d, b);

	return (abc != abd && cda != cdb);
}
