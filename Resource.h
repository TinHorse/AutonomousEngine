#pragma once
#include "ECS.h"

class Resource : public Entity
{
public:
	Resource(EntityManager& manager, enum ResourceType resType, int resAmount) : Entity(manager), ResourceType(resType), resourceAmount(resAmount)
	{

	}
	~Resource() {}

	void Update() override
	{
		if (resourceAmount <= 0)
		{
			OnDeath();
		}
	}

	enum ResourceType ResourceType;
	int resourceAmount = 0;

	void AddCurrentlyHarvestingComponent(Component* comp)
	{
		componentsCurrentlyHarvesting.push_back(comp);
	}

	void OnDeath()
	{
		for (Component* comp : componentsCurrentlyHarvesting)
		{
			comp->NotifyOfEntityDestroyed(this);
		}
		this->Destroy();
	}

private:
	std::vector<Component*> componentsCurrentlyHarvesting;
};