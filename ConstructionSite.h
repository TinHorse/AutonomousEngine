#pragma once
#include "ECS.h"
#include "Components.h"
#include "ConfigCosts.h"

class ConstructionSite : public Building
{
public:
	ConstructionSite() = default;

	ConstructionSite(EntityManager& manager, BuildingType type) : Building(manager)
	{
		buildingType = type;
		EntityCosts costs;
		resources_needed = costs.BuildingCosts[type];
		totalAmountResourcesNeeded = resources_needed.GetTotalResourceAmount();
	}

	bool ShouldRequestResources()
	{
		int amountResources = GetComponent<StorageComponent>().GetTotalResourceAmount();
		if (amountResources >= totalAmountResourcesNeeded)
		{
			bIsFinished = true;
			return false;
		}


		NumActiveDeliveries -= (amountResources - previousResources);

		if (NumActiveDeliveries >= 5) return false;
		previousResources = GetComponent<StorageComponent>().GetTotalResourceAmount();
		return true;
	}

	~ConstructionSite() {}

	bool MarkedForDestruction = false;
	ResourceHeap resources_needed;

	int NumActiveDeliveries = 0;

	int totalAmountResourcesNeeded = 0;
	int previousResources = 0;

	bool bIsFinished = false;

};