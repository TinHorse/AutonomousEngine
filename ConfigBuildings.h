#pragma once
#include <map>
#include "Game.h"

struct BuildingSize
{
	int w, h;
	float scale;
};

struct ConfigBuildings
{
	ConfigBuildings()
	{
		InitializeBuildings();
	}

	void InitializeBuildings()
	{
		BuildingSize size;
		BuildingType type;

		type = Base;
		size.w = 712;
		size.h = 712;
		size.scale = 0.12f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "base";

		type = Gaslight;
		size.w = 400;
		size.h = 400;
		size.scale = 0.05f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "gaslight";

		type = Syphon;
		size.w = 395;
		size.h = 397;
		size.scale = 0.1f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "syphon";

		type = Scraper;
		size.w = 301;
		size.h = 452;
		size.scale = 0.15f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "scraper";

		type = Harvester;
		size.w = 396;
		size.h = 396;
		size.scale = 0.15f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "harvester";

		type = GatheringStation;
		size.w = 396;
		size.h = 396;
		size.scale = 0.12f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "gathering_station";

		type = Wiringworks;
		size.w = 713;
		size.h = 713;
		size.scale = 0.07f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "wiringworks";

		type = MachineFactory;
		size.w = 800;
		size.h = 800;
		size.scale = 0.085f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "machine_factory";

		type = Accelerator;
		size.w = 400;
		size.h = 400;
		size.scale = 0.07f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "accelerator";

		type = Lighthouse;
		size.w = 400;
		size.h = 400;
		size.scale = 0.07f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "lighthouse";

		type = Growhouse;
		size.w = 800;
		size.h = 800;
		size.scale = 0.1f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "growhouse";

		type = Smithy;
		size.w = 800;
		size.h = 800;
		size.scale = 0.07f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "smithy";

		type = Hangar;
		size.w = 800;
		size.h = 475;
		size.scale = 0.1f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "hangar";

		type = CuttingRig;
		size.w = 452;
		size.h = 453;
		size.scale = 0.14f;
		BuildingSizes[type] = size;
		BuildingSpriteNames[type] = "cutting_rig";
	}

	std::map<BuildingType, BuildingSize> BuildingSizes;
	std::map<BuildingType, std::string> BuildingSpriteNames;
};