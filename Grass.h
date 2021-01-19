#pragma once
#include "Components.h"
#include "StateMachine.h"

class Grass : public Entity
{
public:
	Grass(EntityManager& mManager) : Entity(mManager) {}

	Grass() = default;
	~Grass() {}

	void update() override
	{
		auto ticks = incrementTicks();
		if (ticks % i_state_update == 0)
		{
			if (food <= 0)
			{
				Destroy();
			}
		}
	}

	int& getFood()
	{
		return food;
	}

private:
	int food = 100;
};