#pragma once
#include "Components.h"
#include "Vector2D.h"
#include <vector>
#include "NavigationManager.h"
#include <queue>
#include "Math.h"

extern NavigationManager nav;

class PathfindingComponent : public Component
{
public:
	PathfindingComponent() {}
	~PathfindingComponent() {}

	void Init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Update() override
	{
		if (moving)
		{
			if (Math::distance(transform->position, target) < 10)
			{
				// if next target doesnt exist, set moving to false and clear target
				if (storedPath.empty())
				{
					moving = false;
					target = transform->position;
					return;
				}
				// else, pull up next target
				target = storedPath.front();
				transform->velocity = (target - transform->position).Normalize() * transform->speed;
				storedPath.pop();
			}
		}
	}

	void FindPath(Vector2D target)
	{
		storedPath = std::move(nav.CalculatePath(transform->position, target));
		moving = true;
	}

private:
	TransformComponent *transform;
	std::queue<Vector2D> storedPath;
	bool moving;
	Vector2D target;

};