#pragma once
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

static Result s_explore(Entity * entity)
{
	if (entity->GetComponent<PathfindingComponent>().isStopped())
	{
		pathfindingQueue.makePathfindingRequest(entity);
	}
	return rSUCCESS;
}

static Result s_followDynamicTarget(Entity * entity, Entity * target, Vector2D& targetPosition, const float& min_radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped() || Math::distanceNoSqrt(targetPosition, target->GetComponent<TransformComponent>().position) > 40000)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
			targetPosition = target->GetComponent<TransformComponent>().position;
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
}

static Result s_followStaticTarget(Entity * entity, Entity * target, Vector2D& targetPosition, const float& min_radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped())
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
			targetPosition = target->GetComponent<TransformComponent>().position;
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
}


static Result s_transfer_IncDec(Entity * entityInc, Entity * entityDec, int& varInc, int * varDec, int incrementA, int decrementB, int capA, int capB, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entityInc->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius))
		{
			if (varInc < capA && *varDec > capB)
			{
				varInc += incrementA;
				*varDec += decrementB;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}

static Result s_transfer_DecInc(Entity * entityDec, Entity * entityInc, int& varDec, int * varInc, int decrementA, int incrementB, int capA, int capB, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entityInc->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius))
		{
			if (varDec > capA && *varInc < capB)
			{
				varDec += decrementA;
				*varInc += incrementB;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}






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
	if (pathfinder.isStopped())
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
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped())
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
		else if (entity->GetComponent<PathfindingComponent>().isStopped())
		{
			Vector2D safe = entity->GetComponent<TransformComponent>().position;
			safe -= target->GetComponent<TransformComponent>().position;
			pathfindingQueue.makePathfindingRequest(entity, safe);
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
};