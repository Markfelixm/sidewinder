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

Editor::Editor(World &world) : world(world),
							   isEnabled(false),
							   creator({{}, 1.f, 0.1f, 0.1f, false, BLUE, false, false, true})

{
}

void Editor::update(Sidewinder::Camera &camera)
{
	if (isEnabled && CheckCollisionPointRec(GetMousePosition(), (Rectangle){20.f, 20.f, 200.f, 465.f}))
		return;

	panCamera(camera);
	Vector2 mouseInWorld = camera.screenToWorld(GetMousePosition());

	creator.isClosable = (creator.vertices.size() > 2 && circleContains(creator.vertices.front(), validRadius, mouseInWorld));
	// TODO: fix: cant close when last segment would make concave shape
	// creator.isClosable = creator.isClosable && !isSelfIntersecting(creator.vertices.front());
	creator.isRemovable = (!creator.vertices.empty() && circleContains(creator.vertices.back(), validRadius, mouseInWorld));
	creator.isPlacable = isValidPosition(mouseInWorld);
	bool isWithinEditorWindow = false;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (creator.isClosable)
		{
			createObstacle();
			creator.vertices.clear();
		}
		else if (creator.isRemovable)
		{
			creator.vertices.pop_back();
			creator.isRemovable = false;
		}
		else if (creator.isPlacable)
			creator.vertices.push_back(mouseInWorld);
	}
}

void Editor::draw(Sidewinder::Camera &camera)
{
	// gui
	if (!isEnabled)
	{
		DrawRectangleRec((Rectangle){20.f, 20.f, 200.f, 50.f}, ColorAlpha(RAYWHITE, 0.67f));
		GuiGroupBox((Rectangle){20.f, 20.f, 200.f, 50.f}, "Editor");
		if (GuiButton((Rectangle){25.f, 25.f, 190.f, 40.f}, "Enable"))
			isEnabled = true;
		return;
	}
	DrawRectangleRec((Rectangle){20.f, 20.f, 200.f, 465.f}, ColorAlpha(RAYWHITE, 0.67f));
	GuiGroupBox((Rectangle){20.f, 20.f, 200.f, 465.f}, "Editor");
	if (GuiButton((Rectangle){25.f, 25.f, 190.f, 40.f}, "Disable"))
	{
		creator.vertices.clear();
		isEnabled = false;
		return;
	}
	GuiGroupBox((Rectangle){25.f, 80.f, 190.f, 95.f}, "World");
	GuiSlider((Rectangle){70.f, 85.f, 100.f, 40.f}, "gravity", TextFormat("%.1f", *world.getPGravity()), world.getPGravity(), 0.f, 3000.f);
	GuiCheckBox((Rectangle){70.f, 130.f, 40.f, 40.f}, "player gravity", world.getPPlayerFeelsGravity());

	GuiGroupBox((Rectangle){25.f, 185.f, 190.f, 290.f}, "Obstacle");
	GuiSlider((Rectangle){70.f, 190.f, 100.f, 40.f}, "mass", TextFormat("%.1f", creator.mass), &creator.mass, 0.1f, 1000.f);
	GuiSlider((Rectangle){70.f, 235.f, 100.f, 40.f}, "stiff", TextFormat("%.2f", creator.stiffness), &creator.stiffness, 0.01f, 1.f);
	GuiSlider((Rectangle){70.f, 280.f, 100.f, 40.f}, "damping", TextFormat("%.3f", creator.damping), &creator.damping, 0.001f, 0.1f);
	GuiCheckBox((Rectangle){70.f, 325.f, 40.f, 40.f}, "stationary", &creator.isStationary);
	GuiColorPicker((Rectangle){30.f, 370.f, 100.f, 100.f}, nullptr, &creator.color);
	DrawRectangle(170, 370, 30, 100, creator.color);

	// obstacle vertices
	if (!creator.vertices.empty())
	{
		if (creator.isClosable)
			DrawCircleV(camera.worldToScreen(creator.vertices.front()), validRadius, GREEN);
		DrawCircleLinesV(camera.worldToScreen(creator.vertices.front()), validRadius, BLACK);
		DrawCircleV(camera.worldToScreen(creator.vertices.front()), 4.f, BLACK);
		if (creator.isRemovable)
			DrawCircleV(camera.worldToScreen(creator.vertices.back()), validRadius, BLACK);
		if (!creator.isPlacable && !creator.isClosable && !creator.isRemovable)
			DrawLineEx(camera.worldToScreen(creator.vertices.back()), GetMousePosition(), 8.f, RED);
		if (creator.isClosable) // snap to closable
			DrawLineV(camera.worldToScreen(creator.vertices.back()), camera.worldToScreen(creator.vertices.front()), BLACK);
		else
		{
			DrawCircleV(GetMousePosition(), 4.f, BLACK);
			DrawLineV(camera.worldToScreen(creator.vertices.back()), GetMousePosition(), BLACK);
		}
	}
	for (size_t i = 1; i < creator.vertices.size(); i++)
	{
		DrawCircleV(camera.worldToScreen(creator.vertices.at(i)), 4.f, BLACK);
		DrawCircleLinesV(camera.worldToScreen(creator.vertices.at(i)), validRadius, BLACK);
		DrawLineV(camera.worldToScreen(creator.vertices.at(i - 1)), camera.worldToScreen(creator.vertices.at(i)), BLACK);
	}
}

void Editor::createObstacle()
{
	// TODO: add creation options like color, mass, isStationary

	std::unique_ptr<SoftBody> pObstacle = std::make_unique<Obstacle>(creator.vertices, creator.mass);
	pObstacle->setStiffness(creator.stiffness);
	pObstacle->setDamping(creator.damping);
	pObstacle->setIsStationary(creator.isStationary);
	pObstacle->setColor(creator.color);

	world.addEntity(std::move(pObstacle));
}

bool Editor::isSelfIntersecting(Vector2 &position)
{
	for (size_t i = 1; creator.vertices.size() > 2 && i < (creator.vertices.size() - 1); i++)
	{
		if (segmentsIntersect(
				creator.vertices.at(i - 1),
				creator.vertices.at(i),
				creator.vertices.back(),
				position))
			return true;
	}
	return false;
}

bool Editor::isValidPosition(Vector2 &position)
{
	for (auto &vertex : creator.vertices)
	{
		if (circleContains(vertex, validRadius, position))
			return false;
	}
	// TODO: add minimum angle between lines threshold to avoid colinears
	// TODO: check against existing entities, check bounding box first
	return !isSelfIntersecting(position);
}

void Editor::panCamera(Sidewinder::Camera &camera)
{
	static bool isPanning = false;
	static Vector2 initialPosition = {0.f, 0.f};
	static Vector2 initialCameraCenter = {0.f, 0.f};
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		initialPosition = GetMousePosition();
		initialCameraCenter = camera.getCameraCenterInWorld();
		isPanning = true;
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		isPanning = false;
	if (isPanning)
	{
		Vector2 displacement = Vector2Subtract(GetMousePosition(), initialPosition);
		camera.setCameraCenterInWorld(Vector2Add(initialCameraCenter, Vector2Scale(displacement, -1.f)));
	}
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
