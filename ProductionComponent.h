#pragma once
#include "ECS.h"
#include "Components.h"
#include "NodeSystem.h"
#include "Resource.h"

class ProductionComponent : public Component
{
public:
	ProductionComponent() = default;

	ProductionComponent(enum ResourceType resource_type, int buildingStorageMax, int productionCycle, ResourceType NeededResource, int total_production_per_needed_resource, bool needsResourcesInArea = true, bool shouldTurn = true)
	{
		resourceType = resource_type;
		BuildingStorageMax = buildingStorageMax;
		ProductionCycle = productionCycle;
		neededResource = NeededResource;
		extra_production_per_food = total_production_per_needed_resource - 1;
		bNeedsResourcesInArea = needsResourcesInArea;
		bShouldTurn = shouldTurn;
	}
	~ProductionComponent() {}

	void Update() override
	{
		Produce();
	}

	void Init() override
	{
		LinkComponentPointers();
	}

	void Produce()
	{
		if (ProductionTimer++ % ProductionCycle == 0)
		{
			if (bIsProducing)
			{
				if (cStorage->GetResourceAmount(resourceType) < BuildingStorageMax)
				{
					if (bNeedsResourcesInArea)
					{
						HarvestFromCurrent();
						return;
					}
					cStorage->IncrementResourceBy(resourceType, 1);
				}
			}

			if (cStorage->GetResourceAmount(resourceType) >= 1 && cStorage->NumActivePickups <= 3)
			{
				if (neededResource != None)
				{
					if (current_extra_production > 0)
					{
						current_extra_production--;
					}
					else if (cStorage->GetResourceAmount(neededResource) > 0)
					{
						cStorage->DecrementResourceBy(neededResource, 1);
						current_extra_production += extra_production_per_food;
					}
					else
					{
						return;
					}
				}

				Building* building = dynamic_cast<Building*>(entity);
				if (!building) return;

				Node* node = building->GetNode();
				if (!node) return;

				Game::node_system->AddRequest(Pickup, resourceType, node);

				cStorage->NumActivePickups++;
				cStorage->DecrementResourceBy(resourceType, 1);
			}

		}
		if (ProductionTimer % (ProductionCycle + (ProductionCycle / 2)) == 0)
		{
			if (neededResource == None) return;
			if (cStorage->GetResourceAmount(neededResource) < 2 && cStorage->NumActiveDeliveries <= 2)
			{
				if (Game::player->GetComponent<PlayerResourceComponent>().GetResourceAmount(neededResource) > 0)
				{
					Building* building = dynamic_cast<Building*>(entity);
					if (!building) return;

					Node* node = building->GetNode();
					if (!node) return;

					Game::node_system->AddRequest(Delivery, neededResource, node);

					cStorage->NumActiveDeliveries++;
					Game::player->GetComponent<PlayerResourceComponent>().DecrementResourceBy(neededResource, 1);
				}
			}
		}

		if (bShouldTurn)
		{
			Turn();
		}
	}

	void Turn()
	{
		if (ProductionTimer % 500 < 100)
		{
			cTransform->angle += 0.5f;
		}
		else if (ProductionTimer % 500 >= 300 && ProductionTimer % 600 < 400)
		{
			cTransform->angle -= 0.5f;
		}
	}

	void LinkComponentPointers() override
	{
		cStorage = &entity->GetComponent<StorageComponent>();
		cTransform = &entity->GetComponent<TransformComponent>();
	}

	ResourceType GetResourceType()
	{
		return resourceType;
	}

	void AddResource(Resource* resource)
	{
		resources_in_range.push_back(resource);
	}

	void HarvestFromCurrent()
	{
		if (resources_in_range.empty())
		{
			currentResource = nullptr;
			bIsProducing = false;
			return;
		}
		if (currentResource)
		{
			currentResource->resourceAmount--;
			cStorage->IncrementResourceBy(resourceType, 1);
			return;
		}
		if (!resources_in_range.empty())
		{
			if (resources_in_range[0])
			{
				currentResource = resources_in_range[0];

				TransformComponent& transform = currentResource->GetComponent<TransformComponent>();
				if (bShouldTurn)
				{
					entity->GetComponent<TransformComponent>().SetViewTarget(transform.position);
				}
				return;
			}
			resources_in_range.erase(resources_in_range.begin());
			return;
		}
	}

	void NotifyOfEntityDestroyed(Entity* entity) override
	{
		if (currentResource == entity)
		{
			currentResource = nullptr;
		}

		for (int i = 0; i < resources_in_range.size(); i++)
		{
			if (resources_in_range[i] == entity)
			{
				resources_in_range.erase(resources_in_range.begin() + i);
			}
		}
	}

private:
	StorageComponent* cStorage = nullptr;
	TransformComponent* cTransform = nullptr;

	int BuildingStorageMax = 5;
	// Minimum amount before pickup can be scheduled

	int ProductionTimer = 0;
	int ProductionCycle = 180;

	bool bIsProducing = true;

	// int for how many production one food unit feeds
	int extra_production_per_food = 0;
	int current_extra_production = 0;

	// Resources
	enum ResourceType resourceType = ResourceType::None;
	std::vector<Resource*> resources_in_range;
	Resource* currentResource = nullptr;
	
	enum ResourceType neededResource = ResourceType::None;
	bool bNeedsResourcesInArea = true;

	bool bShouldTurn = true;
};