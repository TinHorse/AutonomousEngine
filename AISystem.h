#pragma once
#include "Components.h"
#include "EntityManager.h"

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
		type = eFood;
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
		type = eHunted;
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
					if (dynamic_cast<FoodSource*>(e))
					{
						this->GetComponent<PathfindingComponent>().FindPathToTarget(e->GetComponent<TransformComponent>().position, *e);
						entity_found = true;
						break;
					}
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
		type = ePredator;
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
	void Update()
	{

	}

	Entity & AddEntity(EntityType entT)
	{
		Entity *entity;

		switch (entT)
		{
		case(eHunted):
			entity = new Hunted(manager);
			break;
		case(eFood):
			entity = new FoodSource(manager);
			break;
		case(ePredator):
			entity = new Predator(manager);
			break;
		default:
			entity = new Entity(manager);
		}

		std::unique_ptr<Entity> uPtr{ entity };
		entities.emplace_back(std::move(uPtr));
		return *entity;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;

};
