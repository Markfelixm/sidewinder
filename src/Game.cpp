#include "Game.hpp"
#include "Collisions.hpp"

Game::Game()
// :  viewState(SCENE), input(Input()), scene(Scene(input))
{
	PointMass x = PointMass({200.f, 200.f}, 10.f);
	PointMass y = PointMass({180.f, 220.f}, 10.f);
	PointMass z = PointMass({220.f, 220.f}, 10.f);
	std::vector<PointMass> xyz = {x, y, z};
	attacker = std::make_unique<Framed>(xyz, 200.08f);

	PointMass a = PointMass({500.f, 500.f}, 100.f);
	PointMass b = PointMass({600.f, 550.f}, 100.f);
	PointMass c = PointMass({700.f, 500.f}, 100.f);
	PointMass d = PointMass({800.f, 700.f}, 100.f);
	PointMass e = PointMass({400.f, 700.f}, 100.f);
	std::vector<PointMass> points = {a, b, c, d, e};
	defender = std::make_unique<Framed>(points, 2000.03f);

	animated = std::make_unique<Animated>(defender->frame);
	animated->createKeyframe(2.f, {200.f, 800.f}, 0.f);
	animated->createKeyframe(10.f, {1400.f, 800.f}, 0.f);
	animated->createKeyframe(20.f, {1400.f, 200.f}, 90.f);
	animated->createKeyframe(40.f, {200.f, 200.f}, 90.f);
	animated->action = Animated::action::PLAY;
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	Vector2 raymouse = GetMousePosition();
	V2 mouse = V2(raymouse.x, raymouse.y);
	attacker->draw(RED);
	Color color = BLUE;
	if (collides(attacker->actual, defender->actual))
		color = YELLOW;
	defender->draw(color);
	if (contains(defender->actual.points, mouse))
	{
		Edge nearest = findNearestEdge(defender->actual.points, mouse);
		DrawLineEx({nearest.a.position.x, nearest.a.position.y}, {nearest.b.position.x, nearest.b.position.y}, 15.f, ORANGE);
	}
	DrawText(TextFormat("angle: %.2f", defender->frame.getRotation()), 20, 20, 20, BLACK);
	DrawText(TextFormat("init: %.2f", defender->frame.initialRotation), 20, 40, 20, BLACK);
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	attacker->update(deltaTime);
	defender->update(deltaTime);
	animated->update(deltaTime);

	Vector2 raymouse = GetMousePosition();
	V2 mouse = V2(raymouse.x, raymouse.y);

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		defender->frame.setRotation(0.f);
		// attacker->actual.move(mouse - attacker->frame.getCenter());
	}
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		defender->frame.setRotation(-90.f);
		// defender->actual.setPosition(mouse);
		// defender->frame.setPosition(mouse);
	}

	// TODO: make stiffness range 0 to 1, maybe relative to mass
	handleCollision(attacker->actual, defender->actual);
}
