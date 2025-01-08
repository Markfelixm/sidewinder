#pragma once

// TODO: some joint system, e.g.pointmass has bool / reference to point that makes update just copy location of joint
// TODO: use shapes

#include "PointMass.hpp"
#include "Spring.hpp"
// #include "AABB.hpp"
// #include "Camera.hpp"
#include "raylib.h"

struct SpringBox
{
	PointMass a;
	PointMass b;
	PointMass c;
	PointMass d;
	Spring ab;
	Spring bc;
	Spring cd;
	Spring da;
	Spring ac;
	Spring bd;

	SpringBox(Vector2 aPosition, Vector2 bPosition, Vector2 cPosition, Vector2 dPosition, float mass, float stiffness, float damping)
		: a(PointMass(aPosition, mass)),
		  b(PointMass(bPosition, mass)),
		  c(PointMass(cPosition, mass)),
		  d(PointMass(dPosition, mass)),
		  ab(Spring(a, b, stiffness, damping)),
		  bc(Spring(b, c, stiffness, damping)),
		  cd(Spring(c, d, stiffness, damping)),
		  da(Spring(d, a, stiffness, damping)),
		  ac(Spring(a, c, stiffness, damping)),
		  bd(Spring(b, d, stiffness, damping))
	{
	}

	void update(float deltaTime)
	{
		ab.apply();
		bc.apply();
		cd.apply();
		da.apply();
		ac.apply();
		bd.apply();

		a.update(deltaTime);
		b.update(deltaTime);
		c.update(deltaTime);
		d.update(deltaTime);
	}

	void draw()
	{
		DrawLineV(a.position, b.position, YELLOW);
		DrawLineV(b.position, c.position, YELLOW);
		DrawLineV(c.position, d.position, YELLOW);
		DrawLineV(d.position, a.position, YELLOW);
		DrawLineV(a.position, c.position, RED);
		DrawLineV(b.position, d.position, RED);
	}
};
