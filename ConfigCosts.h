#pragma once
#include <map>
#include "Game.h"

struct EntityCosts
{
	EntityCosts()
	{
		InitializeBuildingCosts();
		InitializeTurretCosts();
	}

	void InitializeBuildingCosts()
	{
		// Infrastructure
		BuildingType type = Base;
		ResourceHeap cost;
		cost.rIron = 75;
		cost.rSilicate = 50;
		cost.rObsidian = 30;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Gaslight;
		cost.rAmber = 5;
		cost.rSilicate = 2;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Accelerator;
		cost.rAmber = 10;
		cost.rSilicate = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Lighthouse;
		cost.rAmber = 30;
		cost.rSilicate = 10;
		BuildingCosts[type] = cost;

		// Resource Buildings
		cost.Clear();
		type = Syphon;
		cost.rIron = 5;
		cost.rSilicate = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Scraper;
		cost.rSilicate = 10;
		cost.rObsidian = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Harvester;
		cost.rIron = 10;
		cost.rObsidian = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = GatheringStation;
		cost.rIron = 5;
		cost.rSilicate = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Growhouse;
		cost.rIron = 10;
		cost.rSilicate = 15;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = CuttingRig;
		cost.rIron = 20;
		cost.rSilicate = 10;
		BuildingCosts[type] = cost;

		// Hangar
		cost.Clear();
		type = Hangar;
		cost.rIron = 30;
		cost.rSilicate = 10;
		cost.rObsidian = 15;
		BuildingCosts[type] = cost;

		// Manufacturing
		cost.Clear();
		type = Wiringworks;
		cost.rIron = 20;
		cost.rObsidian = 5;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = MachineFactory;
		cost.rIron = 20;
		cost.rSilicate = 10;
		cost.rObsidian = 10;
		BuildingCosts[type] = cost;

		cost.Clear();
		type = Smithy;
		cost.rIron = 20;
		cost.rSilicate = 5;
		BuildingCosts[type] = cost;
	}

	void InitializeTurretCosts()
	{
		// Transporter
		TurretType type = tTransporter;
		ResourceHeap cost;
		cost.rIron = 5;
		cost.rAmber = 10;
		TurretCosts[type] = cost;
	}


	std::map<BuildingType, ResourceHeap> BuildingCosts;
	std::map<TurretType, ResourceHeap> TurretCosts;

};