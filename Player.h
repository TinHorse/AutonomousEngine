#pragma once
#include "Components.h"
#include "StateMachine.h"

class Player : public Entity
{
public:
	Player(EntityManager& mManager) : Entity(mManager) {}

	Player() = default;
	~Player() {}

	void update() override
	{

	}

	int& getFear()
	{
		return fear;
	}

private:
	int fear = 0;
};