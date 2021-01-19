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
		incrementTicks();
		if (Ticks % i_state_update == 0)
		{
			if (food <= 0)
			{
				carrion--;
				if (carrion <= 0)
				{
					Destroy();
				}
			}
		}
	}

	int& getFood()
	{
		return food;
	}

private:
	int food = 100;
	int carrion = 10;
};