#pragma once
#include "NodeSystem.h"
#include <map>
#include "Player.h"
#include "ConstructionManager.h"
#include "ConfigCosts.h"

struct BuildingManufacturingRequest
{
	BuildingManufacturingRequest(ResourceHeap resources_needed, BuildingType type)
	{
		costs = resources_needed;
		building_type = type;
	}

	ResourceHeap costs;
	BuildingType building_type;
	bool MarkedForDestruction = false;

	BuildingManufacturingRequest& operator= (BuildingManufacturingRequest other)
	{
		costs = other.costs;
		building_type = other.building_type;
		MarkedForDestruction = other.MarkedForDestruction;
		return *this;
	}
};

struct TurretManufacturingRequest
{
	TurretManufacturingRequest(ResourceHeap resources_needed, TurretType type)
	{
		costs = resources_needed;
		turret_type = type;
	}

	ResourceHeap costs;
	TurretType turret_type;
	bool MarkedForDestruction = false;

	TurretManufacturingRequest& operator= (TurretManufacturingRequest other)
	{
		costs = other.costs;
		turret_type = other.turret_type;
		MarkedForDestruction = other.MarkedForDestruction;
		return *this;
	}
};

class ManufacturingManager
{
public:
	ManufacturingManager()
	{
		NumTotalManufactures[Base] = 0;
		NumTotalManufactures[Smithy] = 0;
		NumTotalManufactures[Wiringworks] = 0;
		NumTotalManufactures[MachineFactory] = 0;
		NumTotalManufactures[Hangar] = 0;
	}

	void Update()
	{
		HandleBuildingManufacturingRequests();
		HandleTurretManufacturingRequests();
	}

	void AddManufacture(Entity* manufacture, BuildingType type)
	{
		switch (type)
		{
		case Base:
			engines.push_back(manufacture);
			break;
		case Smithy:
			smithies.push_back(manufacture);
			break;
		case Wiringworks:
			wiringworks.push_back(manufacture);
			break;
		case MachineFactory:
			machine_factories.push_back(manufacture);
			break;
		case Hangar:
			hangars.push_back(manufacture);
			break;
		default:
			throw;
		}

		NumTotalManufactures[type]++;
		
		for (auto b : manufacture->GetComponent<ManufacturingComponent>().GetAvailableBuildings())
		{
			availableBuildings[b] = true;
		}
		for (auto b : manufacture->GetComponent<ManufacturingComponent>().GetAvailableTurrets())
		{
			available_turrets[b] = true;
		}
	}

	void AddBuildingManufacturingRequest(BuildingType type)
	{
		buildingManufacturingRequests.push_back(BuildingManufacturingRequest(Costs.BuildingCosts[type], type));
		DeductResourcesFromPlayer(Costs.BuildingCosts[type], Game::player->GetComponent<PlayerResourceComponent>());
	}

	void AddTurretManufacturingRequest(TurretType type)
	{
		turretManufacturingRequests.push_back(TurretManufacturingRequest(Costs.TurretCosts[type], type));
		DeductResourcesFromPlayer(Costs.TurretCosts[type], Game::player->GetComponent<PlayerResourceComponent>());
	}

	void HandleBuildingManufacturingRequests()
	{
		if (buildingManufacturingRequests.empty()) return;
		for (BuildingManufacturingRequest& request : buildingManufacturingRequests)
		{
			std::vector<Entity*>* manufactures = nullptr;

			BuildingType manufactureType = NoBuilding;
			switch (request.building_type)
			{
			case Base:
			case Smithy:
			case Wiringworks:
			case MachineFactory:
			case Hangar:
				manufactures = &engines;
				manufactureType = Base;
				break;
			case GatheringStation:
			case Syphon:
				manufactures = &smithies;
				manufactureType = Smithy;
				break;
			case Gaslight:
			case Lighthouse:
			case Accelerator:
				manufactures = &wiringworks;		
				manufactureType = Wiringworks;
				break;
			case Scraper:
			case Harvester:
			case CuttingRig:
				manufactures = &machine_factories;
				manufactureType = MachineFactory;
				break;
			default:
				throw;
			}

			for (auto* manu : *manufactures)
			{
				ManufacturingComponent& manufacture = manu->GetComponent<ManufacturingComponent>();
				if (!manufacture.IsWorkingOnRequest())
				{
					if (manufacture.CanProduceBuilding(request.building_type))
					{
						manufacture.ReceiveBuildingRequest(request.building_type, request.costs);
						request.MarkedForDestruction = true;
						break;
					}
				}
			}
		}

		// destroy finished construction sites
		buildingManufacturingRequests.erase(std::remove_if(buildingManufacturingRequests.begin(), buildingManufacturingRequests.end(), // iterates through
			[](BuildingManufacturingRequest& request) // creates unique ptr to each element
		{
			return request.MarkedForDestruction; // for each element, checks if active
		}),
			buildingManufacturingRequests.end());
	}

	void HandleTurretManufacturingRequests()
	{
		if (turretManufacturingRequests.empty()) return;
		for (TurretManufacturingRequest& request : turretManufacturingRequests)
		{
			std::vector<Entity*>* manufactures = nullptr;

			BuildingType manufactureType = NoBuilding;
			switch (request.turret_type)
			{
			case tTransporter:
				manufactures = &hangars;
				manufactureType = Hangar;
				break;
			default:
				throw;
			}

			for (auto* manu : *manufactures)
			{
				ManufacturingComponent& manufacture = manu->GetComponent<ManufacturingComponent>();
				if (!manufacture.IsWorkingOnRequest())
				{
					if (manufacture.CanProduceTurret(request.turret_type))
					{
						manufacture.ReceiveTurretRequest(request.turret_type, request.costs);
						request.MarkedForDestruction = true;
						break;
					}
				}
			}
		}

		// destroy finished construction sites
		turretManufacturingRequests.erase(std::remove_if(turretManufacturingRequests.begin(), turretManufacturingRequests.end(), // iterates through
			[](TurretManufacturingRequest& request) // creates unique ptr to each element
		{
			return request.MarkedForDestruction; // for each element, checks if active
		}),
			turretManufacturingRequests.end());
	}

	bool GetHasSufficientResourcesManufacturing(BuildingType buildingType, TurretType turretType, PlayerResourceComponent* playerResources)
	{
		ResourceHeap* costs;
		if(buildingType != NoBuilding)
			costs = &Costs.BuildingCosts[buildingType];
		else
			costs = &Costs.TurretCosts[turretType];

		if (playerResources->rAmber >= costs->rAmber
			&& playerResources->rIron >= costs->rIron
			&& playerResources->rMorphite >= costs->rMorphite
			&& playerResources->rMushrooms >= costs->rMushrooms
			&& playerResources->rObsidian >= costs->rObsidian
			&& playerResources->rSilicate >= costs->rSilicate)
		{
			return true;
		}
		return false;
	}

	void DeductResourcesFromPlayer(const ResourceHeap& cost, PlayerResourceComponent& playerRes)
	{
		playerRes.rMushrooms -= cost.rMushrooms;
		playerRes.rAmber -= cost.rAmber;
		playerRes.rIron -= cost.rIron;
		playerRes.rSilicate -= cost.rSilicate;
		playerRes.rObsidian -= cost.rObsidian;
		playerRes.rMorphite -= cost.rMorphite;
	}

	void ToggleIsActive()
	{
		bIsActive = bIsActive ? false : true;
	}

	bool GetIsActive()
	{
		return bIsActive;
	}

	void SetIsActive(bool active)
	{
		bIsActive = active;
	}

	bool CanManufactureBuilding(BuildingType type)
	{
		if (availableBuildings.find(type) != availableBuildings.end()) return true;
		return false;
	}

	bool CanManufactureTurret(TurretType type)
	{
		if (available_turrets.find(type) != available_turrets.end()) return true;
		return false;
	}

	int GetNumIdleManufactures(BuildingType type)
	{
		std::vector<Entity*>* manufactures = nullptr;
		switch (type)
		{
		case Base:
			manufactures = &engines;
			break;
		case Smithy:
			manufactures = &smithies;
			break;
		case Wiringworks:
			manufactures = &wiringworks;
			break;
		case MachineFactory:
			manufactures = &machine_factories;
			break;
		case Hangar:
			manufactures = &hangars;
			break;
		}

		int Idles = 0;
		for (auto* e : *manufactures)
		{
			if (!e->GetComponent<ManufacturingComponent>().IsWorkingOnRequest())
				Idles++;
		}
		return Idles;
	}

	int GetNumTotalManufactures(BuildingType type)
	{
		return NumTotalManufactures[type];
	}

private:
	std::vector<BuildingManufacturingRequest> buildingManufacturingRequests;
	std::vector<TurretManufacturingRequest> turretManufacturingRequests;

	std::vector<Entity*> engines;
	std::vector<Entity*> smithies;
	std::vector<Entity*> wiringworks;
	std::vector<Entity*> machine_factories;
	std::vector<Entity*> hangars;

	std::map<BuildingType, int> NumTotalManufactures;

	EntityCosts Costs;
	bool bIsActive = false;
	std::map<BuildingType, bool> availableBuildings;
	std::map<TurretType, bool> available_turrets;
};