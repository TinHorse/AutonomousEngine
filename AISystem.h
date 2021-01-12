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
	rSUCCESS,
	rNO_TARGET
};

static Result s_exploring(Entity * entity, Group targetGroup, float searchDistance)
{
	auto& pathfinder = entity->GetComponent<PathfindingComponent>();
	auto& state = entity->GetComponent<StateComponent>();
	if (!state.getTarget("current"))
	{
		auto& foodSources = manager.FindEntitiesInArea(entity->GetComponent<TransformComponent>().position, targetGroup, searchDistance);
		for (auto& f : foodSources)
		{
			pathfindingQueue.makePathfindingRequest(entity, f);
			state.setTarget("current", f);
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
	return rNO_TARGET;
};

static Result s_transferState(Entity * entity, Entity *target, std::string transfer_state, int rate_increase, int rate_decrease, int max_until_stop, int transfer_range)
{
	if (target)
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
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
};

static Result s_runAway(Entity * entity, Entity * target, float radius)
{
	if (target)
	{
		if (!Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), radius))
		{
			return rSUCCESS;
		}
		else if (!entity->GetComponent<PathfindingComponent>().moving)
		{
			Vector2D safe = entity->GetComponent<TransformComponent>().position;
			safe -= target->GetComponent<TransformComponent>().position;
			pathfindingQueue.makePathfindingRequest(entity, safe);
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
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
					state.pushBehaviour(waitingForCalm, 1);
					state.pushBehaviour(followTarget, 1);
					auto& pathfinder = entity->GetComponent<PathfindingComponent>();
					pathfindingQueue.makePathfindingRequest(entity, state.getTarget("origin"));
					state.setTarget("current", state.getTarget("origin"));
					state.setB("returningToShepherd", 0);
				}
				
				if (state.getB("exploring") > 100)
				{
					state.pushBehaviour(exploring, 0);
					state.setB("exploring", 0);
				}

				if (state.getTarget("enemy"))
				{
					state.pushBehaviour(fleeing, 2);
				}
			}

			if (ticks % 30 == 0)
			{
				// call behaviour
				performBehaviourHunted(entity, ticks);
			}
		}
	}

	void performBehaviourHunted(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();

		//execute state
		Result result;
		switch (state.currentBehaviour())
		{
		case idle:
			//std::cout << "idle" << std::endl;
			break;

		case exploring:
			//std::cout << "exploring" << std::endl;
			if (s_exploring(entity, Game::groupFood, 100))
			{
				state.popBehaviour();
				state.pushBehaviour(eating, 0);
				state.pushBehaviour(followTarget, 0);
			}
			break;

		case followTarget:
			//std::cout << "followTarget " << state.getTarget("current") <<std::endl;
			result = s_followTarget(entity, state.getTarget("current"), 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rNO_TARGET:
				state.popBehaviour();
				break;
			default:
				break;
			}
			break;

		case eating:
			//std::cout << "eating" << std::endl;
			result = s_transferState(entity, state.getTarget("current"), "food", 1, -1, 10, 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				state.setTarget("current", nullptr);
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 0);
				break;
			case rNO_TARGET:
				state.popBehaviour();
			default:
				break;
			}
			break;

		case waitingForCalm:
			//std::cout << "waitingForCalm" << std::endl;
			result = s_transferState(entity, state.getTarget("origin"), "calm", 100, 0, 50, 10);
			switch (result)
			{
			case rSUCCESS:
				state.setTarget("current", nullptr);
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 1);
				break;
			case rCONTINUE:
				break;
			}
			break;

		case fleeing:
			//std::cout << "waitingForCalm" << std::endl;
			result = s_runAway(entity, state.getTarget("enemy"), 200);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				state.setTarget("enemy", nullptr);
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 2);
				break;
			case rCONTINUE:
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
		for (auto& entity : manager.GetGroup(Game::groupPredators))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				auto& state = entity->GetComponent<StateComponent>();

				state.addS("hunger", 1);

				if (state.getS("hunger") > 50)
				{
					state.addB("exploring", 1);
				}

				if (state.getB("exploring") > 20)
				{
					state.pushBehaviour(exploring, 0);
					state.setB("exploring", 0);
				}
			}
			if (ticks % 30 == 0)
			{
				performBehaviourPredators(entity, ticks);
			}
		}
	}

	void performBehaviourPredators(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();

		//execute state
		Result result;
		switch (state.currentBehaviour())
		{
		case idle:
			std::cout << "idle" << std::endl;
			break;

		case exploring:
			std::cout << "exploring" << std::endl;
			if (s_exploring(entity, Game::groupHunted, 200))
			{
				state.popBehaviour();
				state.pushBehaviour(attacking, 0);
				state.pushBehaviour(followTarget, 0);
				state.getTarget("current")->GetComponent<StateComponent>().setTarget("enemy", entity);
			}
			break;

		case followTarget:
			std::cout << "followTarget " << state.getTarget("current") << std::endl;
			result = s_followTarget(entity, state.getTarget("current"), 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rNO_TARGET:
				state.popBehaviour();
				break;
			default:
				break;
			}
			break;

		case attacking:
			std::cout << "attacking" << std::endl;
			result = s_transferState(entity, state.getTarget("current"), "health", 0, -10, 101, 10);
			switch (result)
			{
			case rSUCCESS:
				state.setTarget("current", nullptr);
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 0);
				break;
			case rNO_TARGET:
				state.popBehaviour();
			default:
				break;
			}
			break;
		default:
			assert(true && "FSM trying to access Behaviour function that doesnt exist");
		}
	}
	

private:
	
};
