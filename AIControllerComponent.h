#pragma once
#include "ECS.h"
#include "Components.h"

extern EntityManager manager;

class AIControllerComponent : public Component
{
public:
	AIControllerComponent() {}
	void Init() override
	{
		if (!entity->HasComponent<PathfindingComponent>())
		{
			//entity->AddComponent<PathfindingComponent>();
		}
		//pathfinder = &entity->GetComponent<PathfindingComponent>();
	}

	void Update() override
	{
		
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


private:
	PathfindingComponent *pathfinder;
	std::vector<Entity*> entities;

};