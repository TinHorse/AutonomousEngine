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
	if (entity->GetComponent<PathfindingComponent>().recalcFlag() || state_switch)
	{
		if (!entity->GetComponent<PathfindingComponent>().skip())
		{
			pathfindingQueue.makePathfindingRequest(entity);
		}
	}
	return rSUCCESS;
}

static Result s_followDynamicTarget(bool state_switch, Entity * entity, Entity * target, Vector2D& targetPosition, const float& min_radius, const float max_distance = 0)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (max_distance && Math::distanceNoSqrt(targetPosition, target->GetComponent<TransformComponent>().position) > pow(max_distance, 2))
		{
			target = nullptr;
			return rNO_TARGET;
		}
		else if (entity->GetComponent<PathfindingComponent>().recalcFlag() || Math::distanceNoSqrt(targetPosition, target->GetComponent<TransformComponent>().position) > 40000 || state_switch)
		{
			pathfindingQueue.makePathfindingRequest(entity, target);
			targetPosition = target->GetComponent<TransformComponent>().position;
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
}

static Result s_followStaticTarget(bool state_switch, Entity * entity, Entity * target, const float& min_radius)
{
	if (target)
	{
		if (Collision::AABBExtended(target->GetComponent<ColliderComponent>(), entity->GetComponent<ColliderComponent>(), min_radius))
		{
			entity->GetComponent<PathfindingComponent>().Stop();
			return rSUCCESS;
		}
		else if (entity->GetComponent<PathfindingComponent>().recalcFlag() || state_switch)
		{
			if (!entity->GetComponent<PathfindingComponent>().skip())
			{
				pathfindingQueue.makePathfindingRequest(entity, target);
			}
		}
		return rCONTINUE;
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
		else if (entity->GetComponent<PathfindingComponent>().recalcFlag() || state_switch)
		{
			Vector2D safe = entity->GetComponent<TransformComponent>().position;
			safe -= enemy->GetComponent<TransformComponent>().position;
			safe.Normalize() * 200;
			pathfindingQueue.makePathfindingRequest(entity, safe + entity->GetComponent<TransformComponent>().position);
		}
		return rCONTINUE;
	}
	return rNO_TARGET;
};





static Result s_IncThis(bool state_switch, Entity * entity, Entity * entityOther, int & var, int increment, int cap, float max_transfer_radius)
{
	if (entityOther)
	{
		if (Collision::AABBExtended(entity->GetComponent<ColliderComponent>(), entityOther->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
		{
			if (var < cap)
			{
				var += increment;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}

static Result s_DecThis(bool state_switch, Entity * entity, Entity * entityOther, int & var, int decrement, int cap, float max_transfer_radius)
{
	if (entityOther)
	{
		if (Collision::AABBExtended(entity->GetComponent<ColliderComponent>(), entityOther->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
		{
			if (var > cap)
			{
				var += decrement;
				return rCONTINUE;
			}
			return rSUCCESS;
		}
		return rFAIL;
	}
	return rNO_TARGET;
}

static Result s_DecOther(bool state_switch, Entity * entity, Entity * entityDec, int * var, int decrement, int cap, float max_transfer_radius)
{
	if (entityDec)
	{
		if (Collision::AABBExtended(entity->GetComponent<ColliderComponent>(), entityDec->GetComponent<ColliderComponent>(), max_transfer_radius) || state_switch)
		{
			if (*var > cap)
			{
				*var += decrement;
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
