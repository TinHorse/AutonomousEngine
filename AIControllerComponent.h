#pragma once
#include "ECS.h"
#include <random>
#include <stack>

enum BehaviourState : std::size_t
{
	idle,
	exploring,
	following,
	fleeing,
	returning,
	attacking
};

class AIControllerComponent : public Component
{
public:
	Vector2D origin;
	std::stack<BehaviourState> behaviour;
	bool state_change{ true };
	BehaviourState current_state;

	// randomizer
	std::random_device rd;

	// individual state variables
	int tick{ 0 };
	int health{ 100 };
	int serotonin{ 0 };

	int hunger{ 0 };
	int aggro{ 0 };
	int fear{ 0 };

	int tick_hunger_increase{ 500 };
	int tick_health_decrease_from_hunger{ 200 };
	int tick_fear_increase{ 100 };

	int tick_look_for_food{ 100 };

	AIControllerComponent() = default;
	void Init() override
	{
		pathfinder = &entity->GetComponent<PathfindingComponent>();
		behaviour.push(exploring);
		current_state = behaviour.top();
	}

	void Update() override
	{
		tick++;
		// Update state variables
		if (tick % tick_hunger_increase == 0)
		{
			hunger++;
		}
		if (hunger >= 100 && tick % tick_health_decrease_from_hunger == 0)
		{
			health--;
		}
		if (tick % tick_fear_increase == 0)
		{
			fear++;
		}

		// check current state and execute behaviour based on it
		switch (current_state)
		{
		case exploring:
			Explore();
			break;
		default:
			break;
		}

		if (state_change)
		{
			current_state = behaviour.top();
			switch (behaviour.top())
			{
			case exploring:
				Explore();
				break;
			default:
				break;
			}
			behaviour.pop();
			state_change = false;
		}
	}

	Vector2D FindRandomPointInRadius(const Vector2D& position, float radius)
	{
		std::uniform_real_distribution<double> rng(-radius, radius);
		float r1 = rng(rd);
		if (abs(r1) < radius / 2.f)
		{
			r1 *= (radius / r1);
		}
		float r2 = rng(rd);
		if (abs(r2) < radius / 4.f)
		{
			r2 *= (radius / r2);
		}

		Vector2D v(r1, r2);
		v += position;
		return Vector2D(v);
	}

	void Explore()
	{
		if (!pathfinder->moving) // calculate new path if not moving
		{
			Vector2D pos = entity->GetComponent<TransformComponent>().position;
			origin = pos * 0.9f;
			pathfinder->FindPath(FindRandomPointInRadius(entity->GetComponent<TransformComponent>().position, 500.f));
		}

		if (tick % tick_look_for_food == 0)
		{

		}

	}


	AIControllerComponent& operator=(const AIControllerComponent& comp)
	{
		entities = comp.entities;
		pathfinder = comp.pathfinder;
		behaviour = comp.behaviour;
		return *this;
	}

private:
	PathfindingComponent *pathfinder;
	std::vector<Entity*> entities;

};