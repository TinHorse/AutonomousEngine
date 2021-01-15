#pragma once
#include "Components.h"
#include "EntityManager.h"

static void updateHunted(Entity& entity)
{
	auto& state = entity.GetComponent<StateComponent>();

	if (!state.getS("isDead"))
	{
		// update states
		state.addS("calm", -1);
		state.addS("hunger", 1);

		if (state.getS("hunger") > 100)
		{
			state.addS("health", -1);
		}

		if (state.getS("health") <= 0)
		{
			state.setS("carrion", 100);
			state.setS("isDead", 1);
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
			state.pushBehaviour(goToOrigin, 1);
			auto& pathfinder = entity.GetComponent<PathfindingComponent>();
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
	else
	{
		state.addS("carrion", -1);
		if (state.getS("carrion") <= 0)
		{
			entity.Destroy();
		}
	}
}

static void updatePredators(Entity & entity)
{
	auto& state = entity.GetComponent<StateComponent>();

	state.addS("hunger", 1);

	if (state.getS("carrion") > 0)
	{
		state.addS("hunger", -10);
		state.addS("carrion", -10);
	}

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

static void updateFoodSources(Entity& entity)
{
	auto& state = entity.GetComponent<StateComponent>();

	if (state.getS("food") <= 0)
	{
		entity.Destroy();
	}
}