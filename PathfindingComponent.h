#pragma once
#include "Components.h"
#include "Vector2D.h"
#include <vector>
#include "NavigationManager.h"
#include <stack>
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
			if (Math::distance(transform->position, path.top()) < 10)
			{
				// else, pull up next target
				path.pop();
				if (!path.empty())
				{
					Vector2D next = path.top();
					Vector2D p = (next - transform->position);
					Vector2D norm = p.Normalize();
					transform->velocity = norm * transform->speed;
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
		path = nav.CalculatePath(transform->position, target);
		if (!path.empty())
		{
			moving = true;
		}
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	bool moving;

};