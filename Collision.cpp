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

bool Collision::CircularCollision(const Vector2D & vecA, const Vector2D & vecB, float radius)
{
	if (Math::distance(vecA, vecB) < radius)
	{
		return true;
	}
	return false;
}

bool Collision::CircularCollision(const SDL_Rect& rectA, const SDL_Rect& rectB, float radius)
{
	if (Math::distance(rectA.x + (rectA.w / 2), rectA.y + (rectA.h / 2), rectB.x + (rectB.w / 2), rectB.y + (rectB.h / 2)) < radius)
	{
		return true;
	}
	return false;
}

bool Collision::CircularCollision(const ColliderComponent & colA, const ColliderComponent & colB, float radius)
{
	if (CircularCollision(colA.collider, colB.collider, radius))
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
	Vector2D mid(rectA.x + (rectA.w / 2.f), rectA.y + (rectA.h / 2.f));
	if (mid.x > rectB.x + rectB.w)
	{
		return Vector2D(1, 0);
	}
	if (mid.x < rectB.x)
	{
		return Vector2D(-1, 0);
	}
	if (mid.y > rectB.y + rectB.h)
	{
		return Vector2D(0, 1);
	}
	if (mid.y < rectB.y)
	{
		return Vector2D(0, -1);
	}
	else
	{
		return Vector2D(rectA.x + (rectA.w / 2.f) - rectB.x + (rectB.w / 2.f),rectA.y + (rectA.h / 2.f) - rectB.y + (rectB.h / 2.f)).Normalize();
	}

}

Vector2D Collision::CalculateOpposingForce(const ColliderComponent & colA, const ColliderComponent & colB)
{
	return CalculateOpposingForce(colA.collider, colB.collider);
}
