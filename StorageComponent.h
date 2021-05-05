#pragma once
#include "ECS.h"
#include "Game.h"

class StorageComponent : public Component
{
public:
	StorageComponent() = default;
	~StorageComponent() {}

	void Update() override
	{
		
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

	void SetResourceAmount(enum ResourceType type, int amount)
	{
		switch (type)
		{
		case ResourceType::None:
			break;
		case ResourceType::Mushrooms:
			rMushrooms = amount;
			break;
		case ResourceType::Amber:
			rAmber = amount;
			break;
		case ResourceType::Iron:
			rIron = amount;
			break;
		case ResourceType::Silicate:
			rSilicate = amount;
			break;
		case ResourceType::Obsidian:
			rObsidian = amount;
			break;
		case ResourceType::Morphite:
			rMorphite = amount;
			break;
		}
	}

	bool CompareResources(ResourceHeap resource_heap)
	{
		if (rMushrooms < resource_heap.rMushrooms) return false;
		if (rAmber < resource_heap.rAmber) return false;
		if (rIron < resource_heap.rIron) return false;
		if (rSilicate < resource_heap.rSilicate) return false;
		if (rObsidian < resource_heap.rObsidian) return false;
		if (rMorphite < resource_heap.rMorphite) return false;
		return true;
	}

	std::map<enum ResourceType, int> GetMissingResources(ResourceHeap resource_heap)
	{
		std::map<enum ResourceType, int>  missing_resources;
		if (rMushrooms < resource_heap.rMushrooms)
		{
			missing_resources[Mushrooms] = resource_heap.rMushrooms - rMushrooms;
		}
		if (rAmber < resource_heap.rAmber)
		{
			missing_resources[Amber] = resource_heap.rAmber - rAmber;
		}
		if (rIron < resource_heap.rIron)
		{
			missing_resources[Iron] = resource_heap.rIron - rIron;
		}
		if (rSilicate < resource_heap.rSilicate)
		{
			missing_resources[Silicate] = resource_heap.rSilicate - rSilicate;
		}
		if (rObsidian < resource_heap.rObsidian)
		{
			missing_resources[Obsidian] = resource_heap.rObsidian - rObsidian;
		}
		if (rMorphite < resource_heap.rMorphite)
		{
			missing_resources[Morphite] = resource_heap.rMorphite - rMorphite;
		}
		return missing_resources;
	}

	void ClearResources()
	{
		rMushrooms = 0;
		rAmber = 0;
		rIron = 0;
		rSilicate = 0;
		rObsidian = 0;
		rMorphite = 0;
	}

	int GetTotalResourceAmount()
	{
		return rMushrooms + rAmber + rIron + rSilicate + rObsidian + rMorphite;
	}

	// NOTE: not implemented properly
	int TotalAmountResources = 0;

	int rMushrooms = 0;
	int rAmber = 0;
	int rIron = 0;
	int rSilicate = 0;
	int rObsidian = 0;
	int rMorphite = 0;

	int NumActivePickups = 0;
	int NumActiveDeliveries = 0;

private:
	enum ResourceType resourceType = ResourceType::None;
};