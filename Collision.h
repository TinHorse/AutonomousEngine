#pragma once
#include "SDL.h"
#include "Vector2D.h"
#include <array>

struct Rect
{
	std::array<Vector2D, 4> edges;
};

class ColliderComponent;

struct Collision
{
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB); // Axis Aligned Bounding Box
	static bool AABB(const SDL_Rect& rectA, const int& Bx, const int& By, const int& Bw, const int& Bh);

	static bool AABBExtended(const SDL_Rect& rectA, const SDL_Rect& rectB, float extra_radius); // Axis Aligned Bounding Box


	static bool CircularCollision(const Vector2D& vecA, const Vector2D& vecB, float radius); // Distance based collision
	static bool CircularCollision(const SDL_Rect& rectA, const SDL_Rect& rectB, float radius); // Distance based collision


	//static Vector2D SAT(const SDL_Rect& cA, const SDL_Rect& cB);
	static Vector2D SAT(const Rect& cA, const Rect& cB, const SDL_Point& centreA, const SDL_Point& centreB);
	static bool SATQuery(const Rect& cA, const Rect& cB, float radial_scale);


	static Vector2D CalculateOpposingForce(const SDL_Rect& rectA, const SDL_Rect& rectB, const Vector2D& centreA, const Vector2D& centreB, bool dynamic);
};