#pragma once
#include "ECS.h"
#include "Components.h"

extern EntityManager manager;

class AIControllerComponent : public Component
{
public:
	Vector2D origin;
	enum BehaviourState : std::size_t
	{
		exploring,
		following,
		fleeing,
		returning,
		attacking,
		idle
	};

	BehaviourState behaviour;

	AIControllerComponent() {}
	void Init() override
	{
		pathfinder = &entity->GetComponent<PathfindingComponent>();
		origin = {350,350};
		behaviour = exploring;
	}

	void Update() override
	{
		if (behaviour == exploring)
		{
			Explore();
		}
	}

	std::vector<Entity*> FindEntitiesInArea(Game::groupLabels l, float dist)
	{
		entities.clear();
		for (auto& e : manager.GetGroup(l))
		{
			if (Math::distance(e->GetComponent<TransformComponent>().position, entity->GetComponent<TransformComponent>().position) < dist)
			{
				entities.emplace_back(e);
			}
		}
		return entities;
	}

	void Explore()
	{
		if (!pathfinder->moving)
		{
			Vector2D pos = entity->GetComponent<TransformComponent>().position;
			Vector2D target = (pos - origin).Normalize() * 200.f;
			target += entity->GetComponent<TransformComponent>().position;
			pathfinder->FindPath(target);
			std::cout << "finding" << std::endl;
		}
	}
	
	void SwitchState()
	{
		if (behaviour == exploring)
		{
			Vector2D pos = entity->GetComponent<TransformComponent>().position;
			origin = pos * 0.9f;
		}
	}

private:
	PathfindingComponent *pathfinder;
	std::vector<Entity*> entities;

};