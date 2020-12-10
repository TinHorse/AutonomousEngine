#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	if (
		rectA.x + rectA.w > rectB.x &&
		rectA.y + rectA.h > rectB.y &&
		rectB.x + rectB.w > rectA.x &&
		rectB.y + rectB.h > rectA.y
		)
	{
		return true;
	}
	return false;
}

bool Collision::AABB(const SDL_Rect& rectA, const int& Bx, const int& By, const int& Bw, const int& Bh)
{
	if (
		rectA.x + rectA.w > Bx &&
		rectA.y + rectA.h > By &&
		Bx + Bw > rectA.x &&
		By + Bh > rectA.y
		)
	{
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent & colA, const ColliderComponent & colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		return true;
	}
	return false;
}

bool Collision::SAT(const ColliderComponent & colA, const ColliderComponent & colB)
{
	return false;
}

Vector2D Collision::CalculateOpposingForce(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (
		rectA.x + rectA.w > rectB.x &&
		rectA.y + rectA.h > rectB.y &&
		rectB.x + rectB.w > rectA.x &&
		rectB.y + rectB.h > rectA.y
		) {
	}

	//Vector2D force;
	if (rectA.x + rectA.w > rectB.x + rectB.w && rectA.y > rectB.y)
	{
		return Vector2D(1.5f, 0);
	}
	else if (rectA.y + rectA.h > rectB.y + rectB.h && rectA.x +rectA.w < rectB.x + rectB.w)
	{
		return Vector2D(0, 1.5f);
	}
	else if (rectA.x < rectB.x && rectA.y + rectA.h < rectB.y + rectB.h)
	{
		return Vector2D(-1.5f, 0);
	}
	else if (rectA.y < rectB.y && rectA.x > rectB.x)
	{
		return Vector2D(0, -1.5f);
	}
	else
	{
		return Vector2D(0, 0);
	}

}

Vector2D Collision::CalculateOpposingForce(const ColliderComponent & colA, const ColliderComponent & colB)
{
	return CalculateOpposingForce(colA.collider, colB.collider);
}
