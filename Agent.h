#pragma once
#include "Components.h"
#include "StateMachine.h"
#include "AIBehaviour.h"

class Agent : public Entity
{
public:
	Agent(EntityManager& mManager) : Entity(mManager) {}
	void update() override
	{
		incrementTicks();
		if (!isDead)
		{
			if (Ticks % i_state_update == 0)
			{
				updateState();
			}
			if (Ticks % i_query_update == 0)
			{
				updateQueries();
			}
			if (Ticks % i_behaviour_update == 0)
			{
				animation = "";
				updateBehaviour();
			}
		}
		else
		{
			if (Ticks % i_state_update == 0)
			{
				whenDead();
			}
		}

		// Play animation
		GetComponent<SpriteComponent>().PlayAnim(animation, Ticks);
	}
	virtual void updateState() = 0;
	virtual void updateQueries() = 0;
	virtual void updateBehaviour() = 0;
	virtual void whenDead() = 0;

	void setAnimation(const char * anim)
	{
		animation = anim;
	}

protected:
	bool state_switch = false;
	bool isDead = false;
	int health;
	Result result;

private:
	std::string animation = "";
};