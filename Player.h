#pragma once
#include "Components.h"
#include "StateMachine.h"

class Player : public Agent
{
public:
	Player(EntityManager& mManager) : Agent(mManager) {}

	Player() = default;
	~Player() {}

	enum Behaviour : std::size_t 
	{
		walking,
		eating,
		feeding,
		attacking
	};

	void update() override
	{
		//switch(this->GetComponent<KeyboardController>().)
	}

	void updateState() override
	{
		
	}

	void updateQueries() override
	{

	}

	void updateBehaviour() override
	{
		
	}

	void whenDead() override
	{
		
	}

	int& getFear()
	{
		return fear;
	}

private:
	int fear = 0;

	StateMachine<Behaviour> machine;
	Behaviour currentState;
};