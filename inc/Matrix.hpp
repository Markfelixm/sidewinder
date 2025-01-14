#pragma once

#include "Vector.hpp"
#include <cmath>
#include <numbers>

struct M22
{
	float m00;
	float m01;
	float m10;
	float m11;

	M22(float m00, float m01, float m10, float m11) : m00(m00), m01(m01), m10(m10), m11(m11) {}

	M22(float angle = 0.f, float scale = 1.f)
	{
		float radians = angle * std::numbers::pi / 180.f;
		float cosTheta = std::cos(radians) * scale;
		float sinTheta = std::sin(radians) * scale;

		m00 = cosTheta;
		m01 = -sinTheta;
		m10 = sinTheta;
		m11 = cosTheta;
	}

	V2 transform(const V2 &vector) const
	{
		return V2(
			vector.x * m00 + vector.y * m01,
			vector.x * m10 + vector.y * m11);
	}

	M22 inverse() const
	{
		float determinant = m00 * m11 - m01 * m10;
		if (determinant == 0.f)
			return M22();

		return M22(
			m11 / determinant, -m01 / determinant,
			-m10 / determinant, m00 / determinant);
	}
};
