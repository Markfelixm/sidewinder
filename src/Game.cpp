#include "Game.hpp"
#include "Collisions.hpp"
#include "Planet.hpp"

void addOrbitKeyframes(Animated *animated, V2 center, float radius, float totalTime, size_t keyframeCount);

Game::Game() : viewport(Viewport(1600, 1000, {800.f, 500.f})), input(Input())
{

	planet = std::make_unique<Planet>(V2(1400.f, 1400.f), 500.f, 3000.f, 50);
	moon = std::make_unique<Planet>(V2(2200.f, 1400.f), 100.f, 600.f, 10);
	shuttle = std::make_unique<Shuttle>(input, V2(800.f, 500.f), 0.f);
	viewport.target = &shuttle->shuttle.frame;

	orbit = std::make_unique<Animated>(moon->planet.frame);
	addOrbitKeyframes(orbit.get(), planet->planet.frame.getCenter(), 800.f, 30.f, 30);
	orbit->action = Animated::action::LOOP;
}

// TODO: move circular point creator to math utils, use for orbit and planet creation
void addOrbitKeyframes(Animated *animated, V2 center, float radius, float totalTime, size_t keyframeCount)
{
	float time = 0.f;
	float timestep = totalTime / keyframeCount;

	for (size_t i = 0; i < keyframeCount; i++)
	{
		float angle = (2.f * std::numbers::pi * i) / keyframeCount;
		V2 position = {center.x + radius * cosf(angle), center.y + radius * sinf(angle)};
		time += timestep;
		animated->createKeyframe(time, position, angle * std::numbers::pi / 180.f);
	}
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	planet->draw(viewport, LIME);
	moon->draw(viewport, BLUE);
	shuttle->draw(viewport, RED);

	DrawText(TextFormat("looping: %.2f", orbit->elapsed), 10, 10, 10, BLACK);

	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	input.update();

	planet->update(deltaTime);
	moon->update(deltaTime);
	shuttle->shuttle.actual.acceleration += planet->gravitationalForce(shuttle->shuttle.actual.getCenter());
	shuttle->update(deltaTime);
	orbit->update(deltaTime);

	// Vector2 raymouse = GetMousePosition();
	// V2 mouse = V2(raymouse.x, raymouse.y);

	// if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	// 	attacker->frame.setPosition(mouse);

	handleCollision(shuttle->shuttle.actual, planet->planet.actual);
	handleCollision(shuttle->shuttle.actual, moon->planet.actual);

	// zoom out when shuttle velocity increases
	// viewport.rotation = shuttle->shuttle.frame.getRotation() * -1.f;
	viewport.update();
}
