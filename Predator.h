#pragma once
#include "Components.h"
#include "StateMachine.h"
#include "Entities.h"

class Predator : public Entity
{
public:
	Predator(EntityManager& mManager) : Entity(mManager){}

	Predator() = default;

	enum Behaviour : std::size_t
	{
		idle,
		exploring,
		attacking,
		eating,
		followingDynamicTarget,
	};

	void update() override
	{
		auto ticks = incrementTicks();
		if (ticks % i_state_update == 0)
		{
			if (!isDead)
			{
				hunger++;

				if (hunger > 100)
				{
					health--;
				}
				if (health <= 0)
				{
					isDead = true;
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
		}
		if (ticks % i_behaviour_update == 0)
		{
			currentState = state.current();
			updateBehaviour();
			if (state.current() != currentState)
			{
				state_switch = true;
			}
			else
			{
				state_switch = false;
			}
		}
	}

	void updateBehaviour()
	{
		switch (state.current())
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
			result = s_followDynamicTarget(state_switch, this, target, targetPosition, 30);
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
						std::cout << temp->getHealth() << " health" << std::endl;
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
			result = s_transfer_IncDec(state_switch, this, target, food, targetValue, 50, -50, 100, 0, 20);
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
	}

	void refresh(std::set<Entity*> deletedEntities) override
	{
		if (deletedEntities.find(target) != deletedEntities.end())
		{
			target = nullptr;
		}
	}

private:
	int health = 100;
	int hunger = 50;
	int food = 0;

	bool isDead = false;

	int b_exploring = 100;

	Entity * target = nullptr;

	Vector2D targetPosition;
	Result result;
	int * targetValue;

	StateMachine<Behaviour> state;
	bool state_switch = false;
	Behaviour currentState;
};

