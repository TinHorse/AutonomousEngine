#pragma once
#include "Components.h"
#include "EntityManager.h"
#include "Queues.h"
#include "Collision.h"

extern EntityManager manager;
extern PathfindingQueue pathfindingQueue;

enum Result : std::size_t 
{
	rCONTINUE,
	rFAIL,
	rSUCCESS
};

static Result s_exploring(Entity * entity)
{
	auto& pathfinder = entity->GetComponent<PathfindingComponent>();
	if (!pathfinder.getTarget("current"))
	{
		auto& foodSources = manager.FindEntitiesInArea(entity->GetComponent<TransformComponent>().position, Game::groupFood, 100);
		for (auto& f : foodSources)
		{
			pathfindingQueue.makePathfindingRequest(entity, f);
			pathfinder.setTarget("current", f);
			return rSUCCESS;
		}
	}
	if (!pathfinder.moving)
	{
		pathfindingQueue.makePathfindingRequest(entity);
	}
	return rCONTINUE;
};

static Result s_followTarget(Entity * entity, Entity * target, float radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), radius))
		{
			return rSUCCESS;
		}
		else if (!entity->GetComponent<PathfindingComponent>().moving)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
		}
		return rCONTINUE;
	}
	return rFAIL;
};

static Result s_transferState(Entity * entity, Entity *target, std::string transfer_state, int rate_increase, int rate_decrease, int max_until_stop, int transfer_range)
{
	if (target && Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), transfer_range))
	{
		auto& state = entity->GetComponent<StateComponent>();
		if (state.getS(transfer_state) < max_until_stop && target->GetComponent<StateComponent>().getS(transfer_state) > 0)
		{
			target->GetComponent<StateComponent>().addS(transfer_state, rate_decrease);
			state.addS(transfer_state, rate_increase);
			return rCONTINUE;
		}
		entity->GetComponent<PathfindingComponent>().setTarget("current", nullptr);
		return rSUCCESS;
	}
	return rFAIL;
};

class AISystem
{
public:
	AISystem()
	{
		
	}
	~AISystem() {}

	void Update()
	{
		updateHunted();
		updateFoodSources();
		updatePredators();
	}

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
					state.addB("exploring", 50);
				}


				// update behaviour based on state
				if (state.getB("returningToShepherd") > 100)
				{
					state.pushBehaviour(transferStateB, 1);
					state.pushBehaviour(followTarget, 1);
					auto& pathfinder = entity->GetComponent<PathfindingComponent>();
					pathfindingQueue.makePathfindingRequest(entity, pathfinder.getTarget("origin"));
					pathfinder.setTarget("current", pathfinder.getTarget("origin"));
					state.setB("returningToShepherd", 0);
				}
				
				if (state.getB("exploring") > 100)
				{
					if (!state.currentPriority())
					{
						state.pushBehaviour(exploring, 0);
					}
					state.setB("exploring", 0);
				}
			}

			if (ticks % 30 == 0)
			{
				// call behaviour
				performBehaviourHunted(entity, ticks);
			}
		}
	}

	Result result;
	void performBehaviourHunted(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();
		auto& pathfinder = entity->GetComponent<PathfindingComponent>();

		//std::cout << state.currentBehaviour() << std::endl;
		//execute state
		switch (state.currentBehaviour())
		{
		case idle:
			break;

		case exploring:
			if (s_exploring(entity))
			{
				state.popBehaviour();
				state.pushBehaviour(eating, 0);
				state.pushBehaviour(followTarget, 0);
			}
			break;

		case followTarget:
			result = s_followTarget(entity, pathfinder.getTarget("current"), 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rFAIL:
				state.popBehaviour();
				break;
			default:
				break;
			}
			break;

		case eating:
			result = s_transferState(entity, pathfinder.getTarget("current"), "food", 1, -1, 10, 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 0);
				break;
			default:
				break;
			}
			break;

		case transferStateB:
			result = s_transferState(entity, pathfinder.getTarget("origin"), "calm", 200, 0, 10, 10);
			switch (result)
			{
			case false:
				state.pushBehaviour(followTarget, 1);
				break;
			case true:
				state.popBehaviour();
				break;
			case 2:
				break;
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

				if (state.getS("food") <= 0)
				{
					entity->Destroy();
				}
			}
		}
	}

	void updatePredators()
	{
		
	}
	

private:
	
};
