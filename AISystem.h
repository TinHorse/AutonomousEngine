#pragma once
#include "Components.h"
#include "EntityManager.h"
#include "Queues.h"
#include "Collision.h"

extern EntityManager manager;
extern PathfindingQueue pathfindingQueue;

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

				if (state.getS("hunger") > 50)
				{
					state.addB("exploring", 50);
				}
				if (state.getS("food") > 0)
				{
					state.addS("hunger", -10);
					state.addS("food", -1);
				}
				state.addS("fear", 1);
				state.addS("hunger", 1);

				performBehaviourHunted(*entity, ticks);
			}
		}
	}

	void performBehaviourHunted(Entity& entity, const long& ticks)
	{
		auto& state = entity.GetComponent<StateComponent>();

		if (state.getB("fleeing") > 0)
		{
			if (Collision::CircularCollision(state.getTarget("player")->GetComponent<TransformComponent>().position, entity.GetComponent<TransformComponent>().position, 150))
			{
				state.setB("fleeing", 0);
				state.addS("fear", -10);
				if (state.getS("fear") <= 0)
				{
					state.setB("fleeing", 0);
				}
			}
			else
			{
				pathfindingQueue.makePathfindingRequest(&entity, state.getTarget("player"));
			}
		}

		else if (state.getS("fear") > 50)
		{
			state.setB("fleeing", 1);
			pathfindingQueue.makePathfindingRequest(&entity, state.getTarget("player"));
		}

		else if (state.getB("eating"))
		{
			Entity* foodSource = state.getTarget("food");
			if (foodSource)
			{
				foodSource->GetComponent<StateComponent>().addS("food", -10);
				state.addS("food", 10);
			}
			else
			{
				state.setB("eating", 0);
			}
		}

		else if (state.getB("movingToTarget"))
		{
			auto* foodSource = state.getTarget("food");;
			if (foodSource)
			{
				if (Collision::CircularCollision(foodSource->GetComponent<TransformComponent>().position, entity.GetComponent<TransformComponent>().position, 100))
				{
					state.setB("movingToTarget", 0);
					// replace following with stack -> top()
					state.setB("eating", 1);
				}
			}
		}

		else if (state.getB("exploring") > 10)
		{
			//pathfinder.Explore();
			pathfindingQueue.makePathfindingRequest(&entity);
			if (!state.getTarget("food"))
			{
				auto& foodSources = manager.FindEntitiesInArea(entity.GetComponent<TransformComponent>().position, Game::groupFood, 100);
				for (auto& f : foodSources)
				{
					pathfindingQueue.makePathfindingRequest(&entity, f);
					state.setTarget("food", f);
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
