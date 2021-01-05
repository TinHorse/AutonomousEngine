#pragma once
#include "Components.h"
#include "EntityManager.h"
#include "Collision.h"

extern EntityManager manager;


class Hunted;
class FoodSource;
class Predator;

class AIObject : public Entity
{
public:
	AIObject(EntityManager& man) : Entity(man)
	{
		tick = rand_int(0, 60);
	}
	virtual void Update() = 0;
	long long tick;
};

class FoodSource : public AIObject
{
public:
	FoodSource(EntityManager& man) : AIObject(man)
	{
		
	}

	void Update() override
	{

	}

	int food{ 0 };
};

class Hunted : public AIObject
{
public:
	Hunted(EntityManager& man) : AIObject(man)
	{
		
	}

	void Update() override
	{
		tick++;
		if (tick % 20 == 0)
		{
			hunger++;
		}
		if (tick % 60 == 0 && hunger > 80)
		{
			health--;
		}


		if (tick % 100 == 0)
		{
			bool entity_found = this->GetComponent<PathfindingComponent>().getTargetEntity();
			if (!entity_found && hunger > 25)
			{
				for (auto e : manager.FindEntitiesInArea(this->GetComponent<TransformComponent>().position, Game::groupFood, 100))
				{
					
				}
			}
			if (!entity_found && hunger > 10)
			{
				this->GetComponent<PathfindingComponent>().Explore();
			}
		}
	}

	int health{ 100 };
	int hunger{ 0 };
	int fear{ 0 };
};

class Predator : public AIObject
{
public:
	Predator(EntityManager& man) : AIObject(man)
	{
		
	}

	void Update() override
	{

	}

};


class AISystem
{
public:
	AISystem()
	{
		
	}
	~AISystem() {}
	void updateHunted()
	{
		for (auto& entity : manager.GetGroup(Game::groupHunted))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				auto& state = entity->GetComponent<StateComponent>();

				//state.addS("hunger", 50);

				//if (state.getS("hunger") > 50)
				//{
					//state.addB("exploring", 50);
				//}
				performBehaviourHunted(*entity, ticks);
			}
		}
	}

	void performBehaviourHunted(Entity& entity, long ticks)
	{
		auto& state = entity.GetComponent<StateComponent>();

		if (state.getB("eating"))
		{
			Entity* foodSource = entity.GetComponent<PathfindingComponent>().getTargetEntity();
			if (dynamic_cast<Entity*>(foodSource))
			{
				foodSource->GetComponent<StateComponent>().addS("food", -150);
			}
			else
			{
				entity.GetComponent<PathfindingComponent>().clearTargetEntity();
				state.setB("eating", 0);
			}
		}


		else if (state.getB("movingToTarget"))
		{
			auto* foodSource = entity.GetComponent<PathfindingComponent>().getTargetEntity();
			if (foodSource)
			{
				if (Collision::AABB(entity.GetComponent<ColliderComponent>(), foodSource->GetComponent<ColliderComponent>()))
				{
					state.setB("movingToTarget", 0);
					// replace following with stack -> top()
					state.setB("eating", 1);
				}
			}
		}

		else if (state.getB("exploring") > 10)
		{
			auto& pathfinder = entity.GetComponent<PathfindingComponent>();
			pathfinder.Explore();
			if (!pathfinder.getTargetEntity())
			{
				auto& foodSources = manager.FindEntitiesInArea(entity.GetComponent<TransformComponent>().position, Game::groupFood, 100);
				for (auto& f : foodSources)
				{
					pathfinder.FindPathToTarget(*f);
					state.setB("movingToTarget", true);
					break;
				}
			}
		}
	}


	void updateFoodSources()
	{
		for (auto& entity : manager.GetGroup(Game::groupFood))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				auto& state = entity->GetComponent<StateComponent>();

				state.addS("food", -50);
				if (state.getS("food") <= 0)
				{
					entity->Destroy();
				}
			}
		}
	}
	

private:
	
};
