#pragma once
#include "Components.h"
#include "Vector2D.h"
#include <vector>
#include "NavigationManager.h"
#include <stack>
#include "Math.h"

extern NavigationManager navigationMan;

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
		transform->velocity = { 0,0 };
		if (moving)
		{
			transform->velocity = Vector2D(path.top() - transform->position).Normalize() * transform->speed;
			if (Math::distance(transform->position, path.top()) < 10)
			{
				// else, pull up next target
				path.pop();
				if (path.empty())
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
	{
		path = std::move(navigationMan.CalculatePath(transform->position, target));
		if (!path.empty())
		{
			path.pop();
			next = path.top();
			moving = true;
		}
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	Vector2D next;
	bool moving;

};