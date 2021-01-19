#pragma once
#include "Vector2D.h"
#include <vector>
#include "NavMesh.h"
#include <stack>
#include "Math.h"
#include <set>
#include <cassert>
#include "Components.h"

extern Navmesh navigation;

class PathfindingComponent : public Component
{
public:
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
			transform->addVelocity((next - transform->position).Normalize());
			if (Math::distanceNoSqrt(transform->position, path.top()) < min_distance)
			{
				// else, pull up next target
				path.pop();
				if (!path.empty())
				{
					max_moves = ((Math::distance(path.top(), transform->position)) / transform->speed) * 1.5f;
					if (has_target && path.size() == 1)
					{
						min_distance = 1;
					}
				}
				else
				{
					moving = false;
					transform->velocity = { 0,0 };
					return;
				}
			}

			//std::cout << max_moves << std::endl;
			max_moves--;
			if (max_moves < 0)
			{
				ClearPath();
			}
		}
	}

	void FindPath(const Vector2D& target)
	{
		has_target = false;
		ClearPath();

		navigation.CalculatePath(entity, path, target, true);
		if (!path.empty())
		{
			moving = true;
			max_moves = ((Math::distance(path.top(), transform->position)) / transform->speed) * 1.5f;
		}
	}

	void FindPathToTarget(Entity* target_entity)
	{
		has_target = true;
		ClearPath();

		navigation.CalculatePath(entity, path, target_entity->GetComponent<TransformComponent>().position, true);

		if (!path.empty())
		{
			moving = true;
			max_moves = ((Math::distance(path.top(), transform->position)) / transform->speed) * 1.5f;
		}

	}

	void ClearPath()
	{
		moving = false;
		move_tries = 0;
		min_distance = 400;
		for (int i = 0; i < path.size(); i++)
		{
			path.pop();
		}
	}

	void Explore()
	{
		if (!moving) // calculate new path if not moving
		{
			FindPath(FindRandomPointInRadius(entity->GetComponent<TransformComponent>().position, 300.f));
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

	void pushPoint(Vector2D skew)
	{
		if (!path.empty())
		{
			path.top() += skew;
			//path.push(skew + transform->position);
		}
		else
		{
			path.push(skew + transform->position);
		}
		
	}


	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Stop()
	{
		moving = false;
	}

	const bool& isStopped()
	{
		return !moving;
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	Vector2D next;
	bool moving;
	bool has_target = false;

	// movement tries
	int move_tries;
	int max_moves;
	float min_distance;
};
