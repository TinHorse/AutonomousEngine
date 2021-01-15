#pragma once

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