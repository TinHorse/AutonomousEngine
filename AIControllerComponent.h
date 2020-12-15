#pragma once
#include "ECS.h"
#include "Components.h"
#include <random>
extern EntityManager manager;

enum BehaviourState : std::size_t
{
	exploring,
	following,
	fleeing,
	returning,
	attacking,
	idle
};

class AIControllerComponent : public Component
{
public:
	Vector2D origin;
	BehaviourState behaviour;

	// randomizer
	std::random_device rd;

	AIControllerComponent() {}
	void Init() override
	{
		pathfinder = &entity->GetComponent<PathfindingComponent>();
		origin = {350,350};
		behaviour = exploring;
	}

	void Update() override
	{
		switch (behaviour)
		{
		case exploring:
			Explore();
			break;
		default:
			break;
		}
	}

	std::vector<Entity*> FindEntitiesInArea(Game::groupLabels l, float dist)
	{
		entities.clear();
		for (auto& e : manager.GetGroup(l))
		{
			if (Math::distance(e->GetComponent<TransformComponent>().position, entity->GetComponent<TransformComponent>().position) < dist)
			{
				entities.emplace_back(e);
			}
		}
		return entities;
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
		if (!pathfinder->moving)
		{
			pathfinder->FindPath(FindRandomPointInRadius(entity->GetComponent<TransformComponent>().position, 500.f));
		}
	}

	
	
	void SwitchState()
	{
		if (behaviour == exploring)
		{
			Vector2D pos = entity->GetComponent<TransformComponent>().position;
			origin = pos * 0.9f;
		}
	}

private:
	PathfindingComponent *pathfinder;
	std::vector<Entity*> entities;

};