#pragma once
#include "ECS.h"

class PlayerResourceComponent : public Component
{
public:
	PlayerResourceComponent()
	{
		rMushrooms = 75;
		rIron = 150;
		rObsidian = 75;
		rSilicate = 100;
		rAmber = 80;

		for (int i = 0; i < BuildingType::COUNT; i++)
		{
			buildings[static_cast<BuildingType>(i)] = 0;
		}
		//buildings[Gaslight] = 15;
		buildings[Base] = 1;
		buildings[Wiringworks] = 1;
		buildings[Smithy] = 1;
		//buildings[MachineFactory] = 1;
	}

	const int GetResourceAmount(enum ResourceType type) const
	{
		switch (type)
		{
		case ResourceType::None:
			break;
		case ResourceType::Mushrooms:
			return rMushrooms;
			break;
		case ResourceType::Amber:
			return rAmber;
			break;
		case ResourceType::Iron:
			return rIron;
			break;
		case ResourceType::Silicate:
			return rSilicate;
			break;
		case ResourceType::Obsidian:
			return rObsidian;
			break;
		case ResourceType::Morphite:
			return rMorphite;
			break;
		}
	}

	void IncrementBuilding(BuildingType building)
	{
		buildings[building]++;
	}

	void DecrementBuilding(BuildingType building)
	{
		buildings[building]--;
	}

	void IncrementResourceBy(enum ResourceType type, int amount)
	{
		switch (type)
		{
		case ResourceType::None:
			break;
		case ResourceType::Mushrooms:
			rMushrooms += amount;
			break;
		case ResourceType::Amber:
			rAmber += amount;
			break;
		case ResourceType::Iron:
			rIron += amount;
			break;
		case ResourceType::Silicate:
			rSilicate += amount;
			break;
		case ResourceType::Obsidian:
			rObsidian += amount;
			break;
		case ResourceType::Morphite:
			rMorphite += amount;
			break;
		}
	}

	void DecrementResourceBy(enum ResourceType type, int amount)
	{
		switch (type)
		{
		case ResourceType::None:
			break;
		case ResourceType::Mushrooms:
			rMushrooms -= amount;
			break;
		case ResourceType::Amber:
			rAmber -= amount;
			break;
		case ResourceType::Iron:
			rIron -= amount;
			break;
		case ResourceType::Silicate:
			rSilicate -= amount;
			break;
		case ResourceType::Obsidian:
			rObsidian -= amount;
			break;
		case ResourceType::Morphite:
			rMorphite -= amount;
			break;
		}
	}


	int rMushrooms = 0;
	int rAmber = 0;
	int rIron = 0;
	int rSilicate = 0;
	int rObsidian = 0;
	int rMorphite = 0;

	std::map<BuildingType, int> buildings;

};