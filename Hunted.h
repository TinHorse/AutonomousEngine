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

	void update() override
	{
		auto ticks = incrementTicks();
		if (ticks % i_state_update == 0)
		{
			// Update state variables
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
					//setAnimation("dead");
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
			else
			{
				carrion--;
				if (carrion <= 0)
				{
					std::cout << "deaed" << std::endl;
					this->Destroy();
				}
			}
		}


		// Queries
		if (ticks % i_query_update == 0)
		{
			if (!isDead)
			{
				auto e = manager.FindOneEntityInArea(GetComponent<TransformComponent>().position, Game::groupPredators, 100);
				if (e)
				{
					enemy = e;
					state.push(fleeing, 2);
					std::cout << "fleeing" << std::endl;
				}
			}
		}

		// Update Behaviour
		if (ticks % i_behaviour_update == 0)
		{
			if (!isDead)
			{
				currentState = state.current();
				animation = "";
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

		// Play animation
		GetComponent<SpriteComponent>().PlayAnim(animation);
	}

	void updateBehaviour()
	{
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
				state.push(eating, 0);
				state.push(followingStaticTarget, 0);
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
		case followingStaticTarget:
			result = s_followStaticTarget(state_switch, this, target, targetPosition, 30);
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
			result = s_transfer_IncDec(state_switch, this, target, food, targetValue, 1, -50, 100, 0, 20);
			setAnimation("eat");
			break;
			switch (result)
			{
			case rCONTINUE:
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
			result = s_transfer_DecInc(state_switch, this, target, fear, targetValue, 5, 0, 0, 1, 50);
			break;
		case fleeing:
			result = s_runAway(state_switch, this, enemy, 100);
			switch (result)
			{
			case rCONTINUE:
				break;
			case rSUCCESS:
				state.pop();
			case rFAIL:
				state.pop();
			}
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

	void setAnimation(const char * anim)
	{
		animation = anim;
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
	int hunger = 0;
	int fear = 0;
	int food = 0;
	int carrion = 0;

	bool isDead = false;

	int b_exploring = 100;
	int b_returningToShepherd = 0;
	int b_fleeing = 0;
	
	Entity * target = nullptr;
	Entity * origin = nullptr;
	Entity * enemy = nullptr;

	Vector2D targetPosition;
	Result result;
	int * targetValue;

	StateMachine<Behaviour> state;
	bool state_switch = false;
	Behaviour currentState;

	std::string animation = "";
};

