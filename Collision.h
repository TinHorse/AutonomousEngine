#pragma once
#include "SDL.h"
#include "Vector2D.h"

class ColliderComponent;

struct Collision
{
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB); // Axis Aligned Bounding Box
	static bool AABB(const SDL_Rect& rectA, const int& Bx, const int& By, const int& Bw, const int& Bh);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);


	static bool CircularCollision(const Vector2D& vecA, const Vector2D& vecB, float radius); // Distance based collision
	static bool CircularCollision(const SDL_Rect& rectA, const SDL_Rect& rectB, float radius); // Distance based collision
	static bool CircularCollision(const ColliderComponent& colA, const ColliderComponent& colB, float radius); // Distance based collision


	static bool SAT(const ColliderComponent& colA, const ColliderComponent& colB);


	static Vector2D CalculateOpposingForce(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static Vector2D CalculateOpposingForce(const ColliderComponent& colA, const ColliderComponent& colB);
};