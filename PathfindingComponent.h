#pragma once
//#include "Components.h"
#include "Vector2D.h"
#include <vector>
#include "NavMesh.h"
#include <stack>
#include "Math.h"

extern Navmesh navigation;

class PathfindingComponent : public Component
{
public:
	bool moving;

	PathfindingComponent() : moving(false) {}
	~PathfindingComponent() {}

	void Init() override
	{
		LinkComponentPointers();
	}

	void Update() override
	{
		if (moving)
		{
			next = path.top();
			transform->velocity = (next - transform->position).Normalize() * transform->speed;
			if (Math::distance(transform->position, path.top()) < 10)
			{
				// else, pull up next target
				path.pop();
				if (path.empty())
				{
					moving = false;
					transform->velocity = {0,0};
					targetReached = true;
				}
			}
		}
	}

	void FindPath(const Vector2D& target)
	{
		moving = false;
		targetReached = false;
		//origin = entity->GetComponent<TransformComponent>().position;
		path = std::move(navigation.CalculatePath(transform->position, target, true));
		if (!path.empty())
		{
			path.pop();
			if (!path.empty())
			{
				moving = true;
			}
		}
	}
	
	void FindPathToTarget(Entity& entity)
	{
		target_entity = &entity;
		moving = false;
		targetReached = false;
		//origin = entity->GetComponent<TransformComponent>().position;
		path = std::move(navigation.CalculatePath(transform->position, entity.GetComponent<TransformComponent>().position, true));
		if (!path.empty())
		{
			path.pop();
			if (!path.empty())
			{
				moving = true;
			}
		}
	}

	void Explore()
	{
		if (!moving) // calculate new path if not moving
		{
			FindPath(FindRandomPointInRadius(entity->GetComponent<TransformComponent>().position, 600.f));
		}
	}
	
	Vector2D FindRandomPointInRadius(const Vector2D& position, const float& radius)
	{
		float r1 = rand_float(-radius, radius);
		if (abs(r1) < radius / 2.f)
		{
			r1 *= (radius / r1);
		}
		float r2 = rand_float(-radius, radius);
		if (abs(r2) < radius / 4.f)
		{
			r2 *= (radius / r2);
		}

		Vector2D v(r1, r2);
		v += position;
		return Vector2D(v);
	}
	

	bool PathIsEmpty()
	{
		return path.empty();
	}

	Entity* getTargetEntity()
	{
		return target_entity;
	}

	void clearTargetEntity()
	{
		target_entity = nullptr;
	}

	const bool isTargetReached()
	{
		return targetReached;
	}

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	Vector2D next;
	//Vector2D origin;
	Entity *target_entity = nullptr;
	bool targetReached{ false };
};
