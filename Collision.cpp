#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	//std::cout << "direct ";
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
	//std::cout << "rect ";
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
	//std::cout << "collider ";
	if (AABB(colA.collider, colB.collider))
	{
		return true;
	}
	return false;
}

bool Collision::AABBExtended(const SDL_Rect& rectA, const SDL_Rect& rectB, float extra_radius)
{
	//std::cout << "extended rect ";
	if (
		rectA.x + rectA.w + extra_radius > rectB.x &&
		rectA.y + rectA.h + extra_radius > rectB.y &&
		rectB.x + rectB.w + extra_radius > rectA.x &&
		rectB.y + rectB.h + extra_radius > rectA.y
		)
	{
		return true;
	}
	return false;
}

bool Collision::AABBExtended(const ColliderComponent & colA, const ColliderComponent & colB, float extra_radius)
{
	//std::cout << "extended collider ";
	if (AABBExtended(colA.collider, colB.collider, extra_radius))
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



Vector2D Collision::SAT(const Rect& cA, const Rect& cB, const SDL_Point& centreA, const SDL_Point& centreB)
{
	float overlap = INFINITY;

	for (int a = 0; a < cA.edges.size(); a++)
	{
		// Find axis projection of each edge A
		int b = (a + 1) % cA.edges.size();
		Vector2D axisProj = { -(cA.edges[b].y - cA.edges[a].y), (cA.edges[b].x - cA.edges[a].x) };

		// Find min max of colA
		float min_cA = INFINITY; float max_cA = -INFINITY;
		for (int p = 0; p < cA.edges.size(); p++)
		{
			float q = (cA.edges[p].x * axisProj.x + cA.edges[p].y * axisProj.y);
			min_cA = std::min(min_cA, q);
			max_cA = std::max(max_cA, q);
		}

		// Find min max of colB
		float min_cB = INFINITY; float max_cB = -INFINITY;
		for (int p = 0; p < cB.edges.size(); p++)
		{
			float q = (cB.edges[p].x * axisProj.x + cB.edges[p].y * axisProj.y);
			min_cB = std::min(min_cB, q);
			max_cB = std::max(max_cB, q);
		}

		// Calculate overlap along projected axis
		overlap = std::min(std::min(max_cA, max_cB) - std::max(min_cA, min_cB), overlap);


		if (!(max_cB >= min_cA && max_cA >= min_cB))
			return Vector2D(0, 0);

	}

	// displace colA along the vector between the two object centres
	SDL_Point centB = centreB;
	Vector2D displacement;
	displacement.x = centB.x - centreA.x;
	displacement.y = centB.y - centreA.y;

	displacement * overlap;
	displacement.Normalize() * -1.f;

	return displacement;
}



Vector2D Collision::CalculateOpposingForce(const SDL_Rect & rectA, const SDL_Rect & rectB, const Vector2D& centreA, const Vector2D& centreB, bool dynamic)
{
	float mag = INT_MAX / (Math::distance(centreA, centreB) + 1);

	if (dynamic)
		mag /= 10.f;
	float mag2 = mag / 5.f;

	if (centreA.x > rectB.x + rectB.w)
	{
		if (centreA.y > centreB.y)
			return Vector2D(mag, mag2);
		else
			return Vector2D(mag, -mag2);
	}
	if (centreA.x < rectB.x)
	{
		if (centreA.y > centreB.y)
			return Vector2D(-mag, mag2);
		else
			return Vector2D(-mag, -mag2);
	}
	if (centreA.y > rectB.y + rectB.h)
	{
		if (centreA.x > centreB.x)
			return Vector2D(mag2, mag);
		else
			return Vector2D(-mag2, mag);
	}
	if (centreA.y < rectB.y)
	{
		if (centreA.x > centreB.x)
			return Vector2D(mag2, -mag);
		else
			return Vector2D(-mag2, -mag);
	}
	
	//std::cout << "other" << std::endl;
	return Vector2D(rectA.x + (rectA.w / 2.f) - rectB.x + (rectB.w / 2.f),rectA.y + (rectA.h / 2.f) - rectB.y + (rectB.h / 2.f)).Normalize() * mag;

}

Vector2D Collision::CalculateOpposingForce(const ColliderComponent & colA, const ColliderComponent & colB, const Vector2D& centreA, const Vector2D& centreB, bool dynamic)
{
	return CalculateOpposingForce(colA.collider, colB.collider, centreA, centreB, dynamic);
}
