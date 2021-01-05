#pragma once
#include "Components.h"
#include "EntityManager.h"
#include "Collision.h"

extern EntityManager manager;

class AISystem
{
public:
	AISystem()
	{
		
	}
	~AISystem() {}
	void updateHunted()
	{
		for (auto& entity : manager.GetGroup(Game::groupHunted))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				auto& state = entity->GetComponent<StateComponent>();

				state.addS("hunger", 50);

				if (state.getS("hunger") > 50)
				{
					state.addB("exploring", 50);
				}
				performBehaviourHunted(*entity, ticks);
			}
		}
	}

	void performBehaviourHunted(Entity& entity, long ticks)
	{
		auto& state = entity.GetComponent<StateComponent>();

		if (state.getB("eating"))
		{
			Entity* foodSource = entity.GetComponent<PathfindingComponent>().getTargetEntity();
			if (foodSource)
			{
				foodSource->GetComponent<StateComponent>().addS("food", -150);
			}
			else
			{
				state.setB("eating", 0);
			}
		}


		else if (state.getB("movingToTarget"))
		{
			auto* foodSource = entity.GetComponent<PathfindingComponent>().getTargetEntity();
			if (foodSource)
			{
				if (Collision::AABB(entity.GetComponent<ColliderComponent>(), foodSource->GetComponent<ColliderComponent>()))
				{
					state.setB("movingToTarget", 0);
					// replace following with stack -> top()
					state.setB("eating", 1);
				}
			}
		}

		else if (state.getB("exploring") > 10)
		{
			auto& pathfinder = entity.GetComponent<PathfindingComponent>();
			pathfinder.Explore();
			if (!pathfinder.getTargetEntity())
			{
				auto& foodSources = manager.FindEntitiesInArea(entity.GetComponent<TransformComponent>().position, Game::groupFood, 100);
				for (auto& f : foodSources)
				{
					pathfinder.FindPathToTarget(*f);
					state.setB("movingToTarget", 1);
					break;
				}
			}
		}
	}


	void updateFoodSources()
	{
		for (auto& entity : manager.GetGroup(Game::groupFood))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				auto& state = entity->GetComponent<StateComponent>();

				//state.addS("food", -30);

				if (state.getS("food") <= 0)
				{
					entity->Destroy();
				}
			}
		}
	}
	

private:
	
};
