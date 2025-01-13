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

	once = std::make_unique<Animated>(attacker->frame);
	once->createKeyframe(0.f, {200.f, 200.f}, 0.f);
	once->createKeyframe(30.f, {1400.f, 800.f}, 0.f);

	looping = std::make_unique<Animated>(defender->frame);
	looping->createKeyframe(2.f, {200.f, 800.f}, 0.f);
	looping->createKeyframe(10.f, {1400.f, 800.f}, 0.f);
	looping->createKeyframe(20.f, {1400.f, 200.f}, 90.f);
	looping->createKeyframe(30.f, {200.f, 200.f}, 90.f);
	looping->action = Animated::action::LOOP;
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
	DrawText(TextFormat("angle: %.2f", defender->frame.getRotation()), 20, 20, 14, BLACK);
	DrawText(TextFormat("init: %.2f", defender->frame.initialRotation), 20, 40, 14, BLACK);
	DrawText(TextFormat("once: %.2f", once->elapsed), 20, 80, 14, BLACK);
	DrawText(TextFormat("once action: %i", once->action), 20, 100, 14, BLACK);
	DrawText(TextFormat("looping: %.2f", looping->elapsed), 20, 120, 14, BLACK);
	DrawFPS(GetScreenWidth() - 80, 10);
	EndDrawing();
}

void Game::update(float deltaTime)
{
	attacker->update(deltaTime);
	defender->update(deltaTime);
	once->update(deltaTime);
	looping->update(deltaTime);

	// Vector2 raymouse = GetMousePosition();
	// V2 mouse = V2(raymouse.x, raymouse.y);

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		once->action = Animated::action::PLAY;
	}
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		once->action = Animated::action::PAUSE;
	}

	handleCollision(attacker->actual, defender->actual);
}
