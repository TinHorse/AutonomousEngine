#pragma once
#include "Vector2D.h"
#include <random>

#define PI 3.14159265359

struct Math
{
	static float distance(const Vector2D& vA, const Vector2D& vB)
	{
		return sqrt((vA.x - vB.x) * (vA.x - vB.x) + (vA.y - vB.y) * (vA.y - vB.y));
	}
	static float distance(float Ax, float Ay, float Bx, float By)
	{
		return sqrt((Ax - Bx) * (Ax - Bx) + (Ay - By) * (Ay - By));
	}
	static float distanceNoSqrt(float Ax, float Ay, float Bx, float By)
	{
		return (Ax - Bx) * (Ax - Bx) + (Ay - By) * (Ay - By);
	}
	static float distanceNoSqrt(const Vector2D& vA, const Vector2D& vB)
	{
		return (vA.x - vB.x) * (vA.x - vB.x) + (vA.y - vB.y) * (vA.y - vB.y);
	}

};

static int rand_int(int low, int high)
{
	static std::default_random_engine rd{};
	using Dist = std::uniform_int_distribution<int>;
	static Dist uid{};
	return uid(rd, Dist::param_type{ low,high });
}

static int rand_float(float low, float high)
{
	static std::default_random_engine rd{};
	using Dist = std::uniform_real_distribution<float>;
	static Dist uid{};
	return uid(rd, Dist::param_type{ low,high });
}

static Vector2D rotate_point(const float& cx, const float& cy, const float& angle, Vector2D point)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin
	point.x -= cx;
	point.y -= cy;

	// rotate point
	float xnew = point.x * c - point.y * s;
	float ynew = point.x * s + point.y * c;

	// translate point back
	point.x = xnew + cx;
	point.y = ynew + cy;
	return point;
}

static double toRad(double degrees)
{
	return (degrees * PI / 180.f);
}

static double toDeg(double radians)
{
	float deg = radians * 180.f / PI;
	deg *= -1.f;
	//if (deg < 0) deg += 360;
	return deg;
}

static float angleToYAxisRad(const Vector2D& position)
{
	return atan2((position.x * -1.f), (-1.f * position.y));
}