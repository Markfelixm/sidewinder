#pragma once

#include "Vector.hpp"
#include "Shape.hpp"
#include <vector>

// TODO: math utils?
static float lerp(const float a, const float b, const float t) { return a + t * (b - a); }

struct Keyframe
{
	float time;
	V2 position;
	float rotation;

	Keyframe(float time, V2 position, float rotation) : time(time), position(position), rotation(rotation) {}
};

struct Animated
{
	enum action
	{
		PLAY,  // begin to end
		LOOP,  // loop begin to end
		PAUSE, // pauses time
		END	   // animation is finished, resets time
	};

	Shape &target;
	action action;
	std::vector<Keyframe> keyframes;
	float elapsed;
	size_t nextFrame;

	Animated(Shape &target) : target(target), action(PAUSE), elapsed(0.f), nextFrame(0) {}

	void createKeyframe(float time, V2 position, float rotation)
	{
		keyframes.push_back(Keyframe(time, position, rotation));
		std::sort(keyframes.begin(), keyframes.end(), [](const Keyframe &a, const Keyframe &b)
				  { return a.time < b.time; });
	}

	void update(float deltaTime)
	{
		if (keyframes.size() < 2)
			return;
		if (action == PAUSE || action == END)
			return;

		elapsed += deltaTime;

		// find 2 keyframes around elapsed
		while (nextFrame < keyframes.size() && elapsed > keyframes[nextFrame].time)
			nextFrame++;
		if (nextFrame == keyframes.size())
		{
			if (action != LOOP)
				action = END;
			nextFrame = 0;
			elapsed = 0.f;
			return;
		}
		if (nextFrame == 0)
			return;

		// lerp between current and next Keyframe
		Keyframe &current = keyframes[nextFrame - 1];
		Keyframe &next = keyframes[nextFrame];

		float span = next.time - current.time;
		float t = (elapsed - current.time) / span;

		V2 newPosition = current.position.lerp(next.position, t);
		float newRotation = lerp(current.rotation, next.rotation, t);

		target.setPosition(newPosition);
		target.setRotation(newRotation);
	}
};
