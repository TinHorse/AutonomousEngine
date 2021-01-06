#pragma once
#include "Vector2D.h"
#include <vector>
#include "NavMesh.h"
#include <stack>
#include "Math.h"
#include <set>

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
			transform->addForce((next - transform->position).Normalize());
			if (Math::distance(transform->position, path.top()) < 10)
			{
				// else, pull up next target
				path.pop();
				previous_position = transform->position;
				movement_tries = 0;
				if (path.empty())
				{
					moving = false;
					transform->velocity = {0,0};
					targetReached = true;
				}
			}
			if (Math::distance(transform->position, previous_position) < 20)
			{
				if (movement_tries++ > 60)
				{
					movement_tries = 0;
					moving = false;
					transform->velocity = { 0,0 };
					for (int i = 0; i < path.size(); i++)
					{
						path.pop();
					}
				}
			}
		}
	}

	void FindPath(Entity* requesting_entity, const Vector2D& target)
	{
		movement_tries = 0;
		previous_position = transform->position;

		moving = false;
		targetReached = false;
		//origin = entity->GetComponent<TransformComponent>().position;

		for (int i = 0; i < path.size(); i++)
		{
			path.pop();
		}

		navigation.CalculatePath(requesting_entity, path, target, true);
		if (!path.empty())
		{
			path.pop();
			if (!path.empty())
			{
				moving = true;
			}
		}
	}
	
	void FindPathToTarget(Entity* requesting_entity, Entity* target_entity)
	{
		movement_tries = 0;
		previous_position = transform->position;

		moving = false;
		targetReached = false;
		//origin = entity->GetComponent<TransformComponent>().position;
		
		for (int i = 0; i < path.size(); i++)
		{
			path.pop();
		}

		navigation.CalculatePath(requesting_entity, path, target_entity->GetComponent<TransformComponent>().position, true);
		if (!path.empty())
		{
			path.pop();
			if (!path.empty())
			{
				moving = true;
			}
		}
	}

	void Explore(Entity* requesting_entity)
	{
		if (!moving) // calculate new path if not moving
		{
			FindPath(requesting_entity, FindRandomPointInRadius(entity->GetComponent<TransformComponent>().position, 500.f));
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
	bool targetReached{ false };
	int movement_tries;
	Vector2D previous_position;
};
