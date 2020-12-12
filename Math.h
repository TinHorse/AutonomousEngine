#pragma once
#include "Vector2D.h"

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
