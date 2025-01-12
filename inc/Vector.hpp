#pragma once
#include <cmath>

struct V2
{
	float x, y;

	V2() : x(0.f), y(0.f) {}

	V2(float x, float y) : x(x), y(y) {}

	V2 xy() const { return V2(x, y); }

	V2 yx() const { return V2(y, x); }

	V2 xx() const { return V2(x, x); }

	V2 yy() const { return V2(y, y); }

	V2 operator+(const V2 &other) const { return V2(x + other.x, y + other.y); }

	V2 operator-(const V2 &other) const { return V2(x - other.x, y - other.y); }

	V2 operator*(float scalar) const { return V2(x * scalar, y * scalar); }

	V2 operator/(float scalar) const
	{
		scalar = std::fmax(1e-6f, scalar);
		return V2(x / scalar, y / scalar);
	}

	V2 &operator+=(const V2 &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	V2 &operator-=(const V2 &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	V2 &operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	V2 &operator/=(const float scalar)
	{
		x /= std::fmax(1e-6f, scalar);
		y /= std::fmax(1e-6f, scalar);
		return *this;
	}

	float length() const { return std::sqrt(x * x + y * y); }

	float lengthSquared() const { return x * x + y * y; }

	float dot(const V2 &other) const { return x * other.x + y * other.y; }

	V2 normalize() const
	{
		float magnitude = length();
		if (magnitude > 0.f)
			return V2(x / magnitude, y / magnitude);
		return V2(0.f, 0.f);
	}

	float angle() const { return std::atan2(y, x); }

	float angleBetween(const V2 &other) const
	{
		float dotProduct = dot(other);
		float magnitudes = length() * other.length();
		if (magnitudes == 0.f)
			return 0.f;

		float cosTheta = dotProduct / magnitudes;
		cosTheta = std::fmax(-1.f, std::fmin(1.f, cosTheta));

		return std::acos(cosTheta);
	}
};
