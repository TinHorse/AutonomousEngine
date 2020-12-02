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
			if (Math::distance(transform->position, path.front()) < 10)
			{
				// else, pull up next target
				path.pop();
				if (!path.empty())
				{
					Vector2D next = path.front();
					Vector2D p = (next - transform->position);
					Vector2D norm = p.Normalize();
					transform->velocity = norm * static_cast<float>(transform->speed);
				}
				else
				{
					moving = false;
					transform->velocity = {0,0};
				}
			}
		}
	}

	void FindPath(Vector2D target)
	{
		path = std::move(nav.CalculatePath(transform->position, target));
		moving = true;
	}

private:
	TransformComponent *transform;
	std::queue<Vector2D> path;
	bool moving;

};