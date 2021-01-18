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
		if (ticks % 30 == 0)
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
		if (ticks % 30 == 0)
		{
			updateBehaviour();
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
				s_explore(this);
			}
			break;
		case followingDynamicTarget:
			result = s_followDynamicTarget(this, target, targetPosition, 30);
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
			result = s_transfer_IncDec(this, target, health, targetValue, 0, -10, 1, 0, 20);
			switch (result)
			{
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
			case rCONTINUE:
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
			result = s_transfer_IncDec(this, target, food, targetValue, 5, -5, 100, 0, 20);
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
};

