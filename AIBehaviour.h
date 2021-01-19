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

static Result s_explore(bool state_switch, Entity * entity)
{
	if (entity->GetComponent<PathfindingComponent>().isStopped() || state_switch)
	{
		pathfindingQueue.makePathfindingRequest(entity);
	}
	return rSUCCESS;
}

static Result s_followDynamicTarget(bool state_switch, Entity * entity, Entity * target, Vector2D& targetPosition, const float& min_radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped() || Math::distanceNoSqrt(targetPosition, target->GetComponent<TransformComponent>().position) > 40000 || state_switch)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
			targetPosition = target->GetComponent<TransformComponent>().position;
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
}

static Result s_followStaticTarget(bool state_switch, Entity * entity, Entity * target, Vector2D& targetPosition, const float& min_radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped() || state_switch)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
			targetPosition = target->GetComponent<TransformComponent>().position;
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
}

static Result s_DecOther(bool state_switch, Entity * entity, Entity * entityDec, int * varDec, int decrementB, int capB, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entity->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
		{
			if (*varDec > capB)
			{
				*varDec += decrementB;
				std::cout << *varDec << " var dec " << std::endl;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}


static Result s_transfer_IncDec(bool state_switch, Entity * entityInc, Entity * entityDec, int& varInc, int * varDec, int incrementA, int decrementB, int capA, int capB, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entityInc->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
		{
			if (varInc < capA && *varDec > capB)
			{
				varInc += incrementA;
				*varDec += decrementB;
				std::cout << varInc << " carrion" << std::endl;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}

static Result s_transfer_DecInc(bool state_switch, Entity * entityDec, Entity * entityInc, int& varDec, int * varInc, int decrementA, int incrementB, int capA, int capB, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entityInc->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
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

static Result s_runAway(bool state_switch, Entity * entity, Entity * enemy, float radius)
{
	if (enemy)
	{
		if (!Collision::AABBExtended(enemy->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), radius))
		{
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().isStopped() || state_switch)
		{
			Vector2D safe = entity->GetComponent<TransformComponent>().position;
			safe -= enemy->GetComponent<TransformComponent>().position;
			safe.Normalize() * 200;
			//std::cout << safe << std::endl;
			pathfindingQueue.makePathfindingRequest(entity, safe);
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
};