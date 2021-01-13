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
			transform->addVelocity((next - transform->position).Normalize());
			if (Math::distanceNoSqrt(transform->position, path.top()) < 150)
			{
				// else, pull up next target
				path.pop();
				if (!path.empty())
				{
					max_moves = ((Math::distance(path.top(), transform->position)) / transform->speed) * 1.5f;
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
				std::cout << "clear path" << std::endl;
			}



			// check movement tries
			/*
			float dist = Math::distanceNoSqrt(transform->position, path.top());
			if (abs(dist - previous_dist) < 255)
			{
				move_tries++;
				if (move_tries > 40)
				{
					if (!path.empty())
					{
						move_tries = 0;
						path.pop();
						if (path.empty())
						{
							moving = false;
						}
					}
					if (move_tries > 80)
					{
						ClearPath();
					}
				}
			}
			*/
			/*
			if (!path.empty())
			{
				previous_dist = Math::distanceNoSqrt(transform->position, path.top());
			}
			*/
		}
	}

	void FindPath(const Vector2D& target)
	{
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

	void pushPoint(Vector2D point)
	{
		point += transform->position;
		path.push(point);
		max_moves = ((Math::distance(path.top(), transform->position)) / transform->speed) * 1.5f;
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

	// movement tries
	int move_tries;
	int max_moves;
	float previous_dist;
};
