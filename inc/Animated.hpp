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
		PLAY,	  // once forward thru
		LOOP,	  // loop forwards
		PINGPONG, // forward until end, reverse to start, looping
		PAUSE,	  // can be continued
		END		  // signals animation finished
	};

	Shape &target;
	action action;
	std::vector<Keyframe> keyframes;
	float elapsed;

	Animated(Shape &target) : target(target), action(PAUSE), elapsed(0.f) {}

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

		elapsed += deltaTime;

		if (action == END || action == PAUSE)
			return;

		// find 2 keyframes around elapsed
		static size_t nextFrame = 0;
		while (nextFrame < keyframes.size() && elapsed > keyframes[nextFrame].time)
			nextFrame++;
		if (nextFrame == keyframes.size())
		{
			// TODO: pingpong
			if (action == LOOP)
			{
				nextFrame = 0;
				elapsed = 0.f;
				return;
			}

			action = END;
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
