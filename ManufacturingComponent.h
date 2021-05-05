#pragma once
#include "ECS.h"
#include "NodeSystem.h"
#include "Components.h"

class ManufacturingComponent : public Component
{
public:
	ManufacturingComponent() = default;
	
	ManufacturingComponent(int ProductionCycle, std::vector<BuildingType> availableBuildings,
		std::vector<TurretType> availableTurrets, bool isBase = false)
	{
		productionCycle = ProductionCycle;
		available_buildings = availableBuildings;
		available_turrets = availableTurrets;
		bIsBase = isBase;
	}
	
	~ManufacturingComponent() {}

	void Update() override
	{
		Produce();
	}

	void Init() override
	{
		LinkComponentPointers();
	}

	bool CanProduceBuilding(BuildingType type)
	{
		for (auto b : available_buildings)
		{
			if (b == type) return true;
		}
		return false;
	}

	bool CanProduceTurret(TurretType type)
	{
		for (auto b : available_turrets)
		{
			if (b == type) return true;
		}
		return false;
	}

	void Produce()
	{
		timer++;
		if (bIsProducing)
		{
			// Produce
			if (timer % productionCycle == 0)
			{
				bIsProducing = false;
				bWorkingOnRequest = false;
				NumActiveDeliveries = 0;

				if (producingTurret != tNone)
				{
					MakeTurret();
					producingTurret = tNone;
					return;
				}
			}
		}

		if (bWorkingOnRequest)
		{
			// Get required resources
			if (timer % deliveryCycle == 0)
			{
				int amountResources = cStorage->GetTotalResourceAmount();
				NumActiveDeliveries -= (amountResources - previousResources);
				SendRequests();

				if (!bIsProducing && amountResources >= totalAmountResourcesNeeded)
				{
					cStorage->ClearResources();
					bIsProducing = true;
					totalAmountResourcesNeeded = 0;
					previousResources = 0;
					timer = 0;
				}
			}
		}
	}

	void MakeTurret()
	{
		Entity* newTurret = nullptr;
		Building* building = dynamic_cast<Building*>(entity);
		if (!building) return;

		Node* node = building->GetNode();
		if (!node) return;

		Vector2D position = building->GetComponent<TransformComponent>().position;

		switch (producingTurret)
		{
		case tTransporter:
			newTurret = &Game::assets->CreateTransporter(position);
			break;
		default:
			throw(producingTurret);
		}
		

		Node* storage = Game::node_system->FindClosestStorage(node);

		// if Transporter
		Transporter* transporter = dynamic_cast<Transporter*>(newTurret);
		if (transporter)
		{
			Game::node_system->AddTransporter(transporter);
		}

		newTurret->GetComponent<NodeMovementComponent>().SetPath(Game::node_system->FindPath(node, storage), storage);
		//newTurret->GetComponent<NodeMovementComponent>().setPathStatus(Idle);

	}


	void SendRequests()
	{
		if (NumActiveDeliveries >= 5) return;
		previousResources = cStorage->GetTotalResourceAmount();

		Building* building = dynamic_cast<Building*>(entity);
		Node* node = building->GetNode();

		if (resources_needed.rAmber > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Amber, node);
			resources_needed.rAmber--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rIron > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Iron, node);
			resources_needed.rIron--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rMorphite > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Morphite, node);
			resources_needed.rMorphite--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rMushrooms > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Mushrooms, node);
			resources_needed.rMushrooms--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rObsidian > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Obsidian, node);
			resources_needed.rObsidian--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rSilicate > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Silicate, node);
			resources_needed.rSilicate--; NumActiveDeliveries++; return;
		}
	}

	void LinkComponentPointers() override
	{
		cStorage = &entity->GetComponent<StorageComponent>();
	}

	void ReceiveTurretRequest(TurretType turret_type, ResourceHeap ResourcesNeeded)
	{
		resources_needed = ResourcesNeeded;
		producingTurret = turret_type;
		bWorkingOnRequest = true;
		totalAmountResourcesNeeded = resources_needed.GetTotalResourceAmount();
		producingBuilding = NoBuilding;
	}

	void ReceiveBuildingRequest(BuildingType building_type, ResourceHeap ResourcesNeeded)
	{
		producingBuilding = building_type;
		producingTurret = tNone;
		bWorkingOnRequest = true;
		if (bIsBase)
		{
			bIsProducing = true;
			ResourcesNeeded = ResourceHeap();
			return;
		}
		resources_needed = ResourcesNeeded;
		totalAmountResourcesNeeded = resources_needed.GetTotalResourceAmount();
	}

	bool IsWorkingOnRequest()
	{
		return bWorkingOnRequest;
	}

	std::vector<BuildingType> GetAvailableBuildings()
	{
		return available_buildings;
	}

	std::vector<TurretType> GetAvailableTurrets()
	{
		return available_turrets;
	}

private:
	StorageComponent* cStorage = nullptr;
	ResourceHeap resources_needed;
	TurretType producingTurret;
	BuildingType producingBuilding;

	bool bWorkingOnRequest = false;
	bool bIsProducing = false;
	int timer = 0;
	int productionCycle = 0;

	int deliveryCycle = 30;
	int NumActiveDeliveries = 0;

	int totalAmountResourcesNeeded = 0;
	int previousResources = 0;

	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	bool bIsBase = false;
};