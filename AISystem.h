#pragma once
#include "Components.h"
#include "EntityManager.h"
#include "Queues.h"
#include "Collision.h"

extern EntityManager manager;
extern PathfindingQueue pathfindingQueue;

static bool s_exploring(Entity * entity)
{
	if (!entity->GetComponent<PathfindingComponent>().getTarget("current"))
	{
		auto& foodSources = manager.FindEntitiesInArea(entity->GetComponent<TransformComponent>().position, Game::groupFood, 100);
		for (auto& f : foodSources)
		{
			pathfindingQueue.makePathfindingRequest(entity, f);
			entity->GetComponent<PathfindingComponent>().setTarget("current", f);
			return true;
		}
	}
	pathfindingQueue.makePathfindingRequest(entity);
	return false;
};

static bool s_followTarget(Entity * entity, Entity * target, float radius)
{
	if (target)
	{
		if (Math::distance(target->GetComponent<TransformComponent>().position, entity->GetComponent<TransformComponent>().position) < radius)
		{
			return true;
		}
		else if (!entity->GetComponent<PathfindingComponent>().moving)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
		}
	}
	return false;
};

static bool s_transferState(Entity * entity, Entity *target, std::string transfer_state, int rate_increase, int rate_decrease, int max_until_stop, int transfer_range)
{
	if (target && Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), transfer_range))
	{
		auto& state = entity->GetComponent<StateComponent>();
		if (state.getS(transfer_state) < max_until_stop)
		{
			target->GetComponent<StateComponent>().addS(transfer_state, rate_decrease);
			state.addS(transfer_state, rate_increase);
			return true;
		}
	}
	return false;
};

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

				// update states
				state.addS("calm", -1);
				state.addS("hunger", 1);

				if (state.getS("hunger") > 100)
				{
					state.addS("health", -1);
				}

				if (state.getS("health") <= 0)
				{
					entity->Destroy();
				}

				if (state.getS("food") > 0)
				{
					state.addS("hunger", -10);
					state.addS("food", -1);
				}

				// update behaviour states
				if (state.getS("calm") <= 0)
				{
					state.addB("returningToShepherd", 10);
				}
				if (state.getS("hunger") >= 50)
				{
					state.addB("exploring", 10);
				}


				// initialize behaviour
				if (state.getB("returningToShepherd") > 100)
				{
					state.pushBehaviour(waitForCalm);
					state.pushBehaviour(followTarget);
					auto& pathfinder = entity->GetComponent<PathfindingComponent>();
					pathfindingQueue.makePathfindingRequest(entity, pathfinder.getTarget("origin"));
					pathfinder.setTarget("current", pathfinder.getTarget("origin"));
					state.setB("returningToShepherd", 0);
				}
				else if (state.getB("exploring") > 100)
				{
					//state.pushBehaviour(exploring);
					state.setB("exploring", 0);
				}

				
			}

			if (ticks % 10 == 0)
			{
				// call behaviour
				performBehaviourHunted(entity, ticks);
			}
		}
	}

	void performBehaviourHunted(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();
		auto& pathfinder = entity->GetComponent<PathfindingComponent>();

		//execute state
		switch (state.currentBehaviour())
		{
		case idle:
			break;
		case exploring:
			if (s_exploring(entity))
			{
				state.pushBehaviour(eating);
				state.pushBehaviour(followTarget);
			}
			break;
		case followTarget:
			if (s_followTarget(entity, pathfinder.getTarget("current"), 50))
			{
				state.popBehaviour();
				entity->GetComponent<PathfindingComponent>().Stop();
			}
			break;
		case eating:
			if (s_transferState(entity, pathfinder.getTarget("current"), "food", 1, -1, 10, 10))
			{
				state.popBehaviour();
			}
			break;
		case waitForCalm:
			if (s_transferState(entity, pathfinder.getTarget("origin"), "calm", 200, 0, 10, 10))
			{
				state.popBehaviour();
			}
			else
			{
				pathfindingQueue.makePathfindingRequest(entity, pathfinder.getTarget("origin"));
				pathfinder.setTarget("current", pathfinder.getTarget("origin"));
				state.pushBehaviour(followTarget);
			}
			break;
		default:
			assert(true && "FSM trying to access Behaviour function that doesnt exist");
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
