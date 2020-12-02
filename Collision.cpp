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

bool Collision::AABB(const ColliderComponent & colA, const ColliderComponent & colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		//std::cout << colA.tag << " colliding with " << colB.tag << std::endl;
		return true;
	}
	return false;
}

bool Collision::SAT(const ColliderComponent & colA, const ColliderComponent & colB)
{
	return false;
}
