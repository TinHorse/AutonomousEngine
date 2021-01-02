#pragma once
#include "Components.h"
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
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Update() override
	{
		//transform->velocity.Zero();
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
				}
			}
		}
	}

	void FindPath(const Vector2D& target)
	{
		moving = false;
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

	bool PathIsEmpty()
	{
		return path.empty();
	}

private:
	TransformComponent *transform;
	std::stack<Vector2D> path;
	Vector2D next;

};