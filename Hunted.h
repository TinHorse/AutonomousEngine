#pragma once
#include "Components.h"

class AISystem;

class Hunted
{
public:
	Hunted(Entity * entity, Entity * origin) {}
	Hunted() = default;
	friend AISystem;

	enum HuntedBehaviour : std::size_t
	{
		idle,
		exploring,
		eating,
		followingTarget,
		waitingForCalm,
		fleeing,
		goingToOrigin
	};

	void update()
	{
		if (!isDead)
		{
			fear++;
			hunger++;

			if (hunger > 100)
			{
				health--;
			}
			if (health <= 0)
			{
				carrion = 100;
				isDead = true;
			}
			if (food > 0)
			{
				hunger -= 10;
				food--;
			}

			// update behaviour states
			if (fear >= 100)
			{
				b_returningToShepherd += 10;
			}
			if (hunger >= 50)
			{
				b_exploring += 10;
			}


			// update behaviour based on state
			if (b_returningToShepherd > 100)
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
	}

private:
	Entity * entity;
	int health = 100;
	int hunger = 50;
	int fear = 50;
	int food = 0;
	int carrion = 0;

	bool isDead = false;

	int b_exploring = 100;
	int b_returningToShepherd = 100;
	int b_fleeing = 0;
	
	Entity * target = nullptr;
	Entity * origin;
	Entity * enemy = nullptr;


};

