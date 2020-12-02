#pragma once
#include "Vector2D.h"

struct Math
{
	static float distance(Vector2D& vA, Vector2D& vB)
	{
		return sqrt((vA.x - vB.x) * (vA.x - vB.x) + (vA.y - vB.y) * (vA.y - vB.y));
	}

};