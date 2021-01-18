#pragma once
#include "Components.h"
#include "StateMachine.h"
#include "Entities.h"

class Hunted : public Entity
{
public:
	Hunted(EntityManager& mManager, Entity * _origin) : Entity(mManager)
	{
		origin = _origin;
	}

	Hunted() = default;

	enum HuntedBehaviour : std::size_t
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

	void update() override
	{
		auto ticks = incrementTicks();
		if (ticks % 30 == 0)
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

				if (enemy)
				{
					state.push(fleeing, 2);
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
			target = manager.FindOneEntityInArea(GetComponent<TransformComponent>().position, Game::groupFood, 100);
			if (target)
			{
				auto temp = dynamic_cast<Grass*>(target);
				if (temp)
				{
					targetValue = &temp->getFood();
					targetPosition = temp->GetComponent<TransformComponent>().position;
				}
				state.push(eating, 0);
				state.push(followingStaticTarget, 0);
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
		case followingStaticTarget:
			result = s_followStaticTarget(this, target, targetPosition, 30);
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
			result = s_transfer_IncDec(this, target, food, targetValue, 1, -50, 100, 0, 20);
			break;
			switch (result)
			{
			case rSUCCESS:
				target = nullptr;
				state.pop();
				break;
			case rCONTINUE:
				break;
			case rFAIL:
				state.push(followingStaticTarget, state.currentPriority());
				break;
			case rNO_TARGET:
				state.pop();
				break;
			}
		case goingToOrigin:
			target = origin;
			if (target)
			{
				auto temp = dynamic_cast<Player*>(target);
				if (temp)
				{
					targetValue = &temp->getFear();
					targetPosition = temp->GetComponent<TransformComponent>().position;
					state.push(followingDynamicTarget, 1);
				}
			}
			break;
		case waitingForFear:
			result = s_transfer_DecInc(this, target, fear, targetValue, 5, 0, 0, 1, 50);
			break;
		}
	}

	void refresh(std::set<Entity*> deletedEntities) override
	{
		if (deletedEntities.find(target) != deletedEntities.end())
		{
			target = nullptr;
		}
		if (deletedEntities.find(origin) != deletedEntities.end())
		{
			origin = nullptr;
		}
		if (deletedEntities.find(enemy) != deletedEntities.end())
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
	Entity * origin = nullptr;
	Entity * enemy = nullptr;

	Vector2D targetPosition;
	Result result;
	int * targetValue;

	StateMachine<HuntedBehaviour> state;
};

