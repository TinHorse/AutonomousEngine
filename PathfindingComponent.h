#pragma once
#include "Vector2D.h"
#include <vector>
#include "NavMesh.h"
#include <stack>
#include "Math.h"
#include <set>
#include <cassert>

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
		if (moving && !stop)
		{
			next = path.top();
			increment_adjusted_distance *= 1.05;
			adjusted_distance += increment_adjusted_distance;
			transform->addForce((next - transform->position).Normalize());
			if (Math::distance(transform->position, path.top()) < adjusted_distance)
			{
				// else, pull up next target
				path.pop();

				if (path.empty())
				{
					stop = true;
					moving = false;
					transform->velocity = {0,0};
				}
				else
				{
					increment_adjusted_distance = 1.f / Math::distanceNoSqrt(transform->position, path.top());
					adjusted_distance = 10;
				}
			}
		}
	}

	void FindPath(Entity* requesting_entity, const Vector2D& target)
	{
		moving = false;
		stop = false;

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
				increment_adjusted_distance = 1.f / Math::distanceNoSqrt(transform->position, path.top());
				adjusted_distance = 10;
			}
		}
	}
	
	void FindPathToTarget(Entity* requesting_entity, Entity* target_entity)
	{
		moving = false;
		stop = false;
		
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
				increment_adjusted_distance = 1.f / Math::distanceNoSqrt(transform->position, path.top());
				adjusted_distance = 10;
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

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}


	// TARGETS

	void initTarget(const std::string& t, Entity* entity)
	{
		assert(targetEntities.find(t) == targetEntities.end() && "attempting to INIT target that already exists");
		targetEntities[t] = entity;
	}

	Entity* getTarget(const std::string& t)
	{
		assert(targetEntities.find(t) != targetEntities.end() && "attempting to GET target that doesnt exist");
		return targetEntities[t];
	}

	void setTarget(const std::string& t, Entity* entity)
	{
		assert(targetEntities.find(t) != targetEntities.end() && "attempting to SET target that doesnt exist");
		targetEntities[t] = entity;
	}

	void Stop()
	{
		stop = true;
	}

	const bool& isStopped()
	{
		return stop;
	}


	void UpdateTargetEntities(std::set<Entity*>& deleted_entities)
	{
		for (auto& entity : deleted_entities)
		{
			for (auto& targets : targetEntities)
			{
				if (targets.second == entity)
				{
					targets.second = nullptr;
				}
			}
		}
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	Vector2D next;
	bool stop = false;

	float adjusted_distance;
	float increment_adjusted_distance;

	std::map<std::string, Entity*> targetEntities;
};
