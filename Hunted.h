#pragma once
#include "Components.h"
#include "StateMachine.h"
#include "Entities.h"

class Hunted : public Agent
{
public:
	Hunted(EntityManager& mManager, Entity * _origin) : Agent(mManager)
	{
		origin = _origin;
		health = 100;
	}

	Hunted() = default;

	enum Behaviour : std::size_t
	{
		idle,
		exploring,
		eating,
		followingDynamicTarget,
		followingStaticTarget,
		waitingForFear,
		fleeing,
		goingToOrigin
	};

	void updateState() override
	{
		// Update state variables
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
			GetComponent<PathfindingComponent>().Stop();
		}
		if (food > 0)
		{
			hunger -= 10;
			food--;
		}

		// Update behaviour variables
		if (fear >= 100)
		{
			b_returningToShepherd += 10;
		}
		if (hunger >= 50)
		{
			b_exploring += 10;
		}


		// Update state machine based on behaviour variables
		if (b_returningToShepherd >= 100)
		{
			state.push(goingToOrigin, 1);
			b_returningToShepherd = 0;
		}

		if (b_exploring >= 100)
		{
			state.push(exploring, 0);
			b_exploring = 0;
		}
	}

	void updateQueries() override
	{
		// Queries
		auto e = manager.FindOneEntityInArea(GetComponent<TransformComponent>().position, Game::groupPredators, 100);
		if (e)
		{
			enemy = e;
			state.push(fleeing, 2);
		}
	}

	void updateBehaviour() override
	{
		currentState = state.current();
		switch (currentState)
		{
		case idle:
			break;
		case exploring:
			target = manager.FindOneEntityInArea(GetComponent<TransformComponent>().position, Game::groupFood, 100);
			if (target)
			{
				auto temp = dynamic_cast<Grass*>(target);
				if (temp)
				{
					targetValue = &temp->getFood();
					targetPosition = temp->GetComponent<TransformComponent>().position;
				}
				state.pop();
				state.push(eating, 0);
				state.push(followingStaticTarget, 0);
			}
			else
			{
				s_explore(state_switch, this);
			}
			break;
		case followingDynamicTarget:
			result = s_followDynamicTarget(state_switch, this, target, targetPosition, 20);
			switch (result)
			{
			case rSUCCESS:
				state.pop();
				break;
			case rNO_TARGET:
				target = nullptr;
				state.pop();
				break;
			}
			break;
		case followingStaticTarget:
			result = s_followStaticTarget(state_switch, this, target, 5);
			switch (result)
			{
			case rSUCCESS:
				state.pop();
				break;
			case rNO_TARGET:
				target = nullptr;
				state.push(exploring, 0);
				break;
			}
			break;
		case eating:
			result = s_transfer_IncDec(state_switch, this, target, food, targetValue, 1, -10, 10, 0, 10);
			switch (result)
			{
			case rCONTINUE:
				setAnimation("eat");
				break;
			case rSUCCESS:
				target = nullptr;
				state.pop();
				break;
			case rFAIL:
				state.push(followingStaticTarget, state.currentPriority());
				break;
			case rNO_TARGET:
				state.pop();
				break;
			}
			break;
		case goingToOrigin:
			target = origin;
			if (target)
			{
				auto temp = dynamic_cast<Player*>(target);
				if (temp)
				{
					targetValue = &temp->getFear();
					targetPosition = temp->GetComponent<TransformComponent>().position;
					state.pop();
					state.push(waitingForFear, 1);
					state.push(followingDynamicTarget, 1);
				}
			}
			break;
		case waitingForFear:
			result = s_DecThis(state_switch, this, target, fear, -10, 0, 40);
			switch (result)
			{
			case rCONTINUE:
				break;
			case rSUCCESS:
				state.pop();
				break;
			case rFAIL:
				state.push(followingDynamicTarget, 1);
				break;
			case rNO_TARGET:
				state.pop();
				break;
			}
			break;
		case fleeing:
			result = s_runAway(state_switch, this, enemy, 100);
			switch (result)
			{
			case rCONTINUE:
				break;
			case rSUCCESS:
				state.pop();
				enemy = nullptr;
				break;
			case rFAIL:
				state.pop();
				break;
			}
			break;
		}

		// check state switch
		if (state.current() != currentState)
		{
			state_switch = true;
		}
		else
		{
			state_switch = false;
		}
	}

	void whenDead() override
	{
		setAnimation("dead");
		carrion--;
		if (carrion <= 0)
		{
			this->Destroy();
		}
	}

	void refresh() override
	{
		if (deletedEntities->find(target) != deletedEntities->end())
		{
			target = nullptr;
		}
		if (deletedEntities->find(origin) != deletedEntities->end())
		{
			origin = nullptr;
		}
		if (deletedEntities->find(enemy) != deletedEntities->end())
		{
			enemy = nullptr;
		}
	}

	int& getHealth()
	{
		return health;
	}

	int& getCarrion()
	{
		return carrion;
	}

	void setEnemy(Entity * e)
	{
		enemy = e;
	}

private:
	int hunger = 100;
	int fear = 0;
	int food = 0;
	int carrion = 0;

	int b_exploring = 0;
	int b_returningToShepherd = 0;
	int b_fleeing = 0;
	
	Entity * target = nullptr;
	Entity * origin = nullptr;
	Entity * enemy = nullptr;

	Vector2D targetPosition;
	int * targetValue;

	StateMachine<Behaviour> state;
	Behaviour currentState;
};

