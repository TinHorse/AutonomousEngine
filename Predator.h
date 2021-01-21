#pragma once
#include "Components.h"
#include "StateMachine.h"
#include "Entities.h"

class Predator : public Agent
{
public:
	Predator(EntityManager& mManager) : Agent(mManager)
	{
		health = 100;
	}

	Predator() = default;

	enum Behaviour : std::size_t
	{
		idle,
		exploring,
		attacking,
		eating,
		followingDynamicTarget,
	};

	void updateState() override
	{
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

		// update behaviour state
		if (hunger >= 50)
		{
			b_exploring += 10;
		}

		// update behaviour based on state
		if (b_exploring >= 100)
		{
			state.push(exploring, 0);
			b_exploring = 0;
		}
	}

	void updateQueries() override
	{
		
	}

	void updateBehaviour() override
	{
		currentState = state.current();
		switch (currentState)
		{
		case idle:
			break;
		case exploring:
			target = manager.FindOneEntityInArea(GetComponent<TransformComponent>().position, Game::groupHunted, 200);
			if (target)
			{
				auto temp = dynamic_cast<Hunted*>(target);
				if (temp)
				{
					targetValue = &temp->getHealth();
					targetPosition = temp->GetComponent<TransformComponent>().position;
					temp->setEnemy(this);
				}
				state.push(eating, 0);
				state.push(attacking, 0);
				state.push(followingDynamicTarget, 0);
			}
			else
			{
				s_explore(state_switch, this);
			}
			break;
		case followingDynamicTarget:
			result = s_followDynamicTarget(state_switch, this, target, targetPosition, 20, 300);
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
		case attacking:
			result = s_DecOther(state_switch, this, target, targetValue, -20, 0, 20);
			switch (result)
			{
			case rCONTINUE:
				break;
			case rSUCCESS:
				state.pop();
				if (target)
				{
					auto temp = dynamic_cast<Hunted*>(target);
					if (temp)
					{
						targetValue = &temp->getCarrion();
					}
				}
				break;
			case rFAIL:
				state.push(followingDynamicTarget, state.currentPriority());
				break;
			case rNO_TARGET:
				state.pop();
				break;
			}
			break;
		case eating:
			result = s_transfer_IncDec(state_switch, this, target, food, targetValue, 1, -1, 100, 0, 20);
			switch (result)
			{
			case rSUCCESS:
				state.pop();
				break;
			case rCONTINUE:
				break;
			case rFAIL:
				state.push(followingDynamicTarget, state.currentPriority());
				break;
			case rNO_TARGET:
				state.pop();
				break;
			}
		}
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
		carrion--;
		if (carrion <= 0)
		{
			std::cout << "deaed" << std::endl;
			this->Destroy();
		}
	}

	void refresh() override
	{
		if (deletedEntities->find(target) != deletedEntities->end())
		{
			target = nullptr;
		}
	}

private:
	int hunger = 50;
	int food = 0;
	int carrion = 0;

	int b_exploring = 100;

	Entity * target = nullptr;

	Vector2D targetPosition;
	Result result;
	int * targetValue;

	StateMachine<Behaviour> state;
	Behaviour currentState;
};

