#pragma once
#include "ECS.h"
#include "Components.h"

class Building : public Entity
{
public:
	Building(EntityManager& manager) : Entity(manager) {}
	Building(EntityManager& manager, BuildingType type, float _range) : Entity(manager), buildingType(type), range(_range)
	{
		
	}
	

	Node* GetNode()
	{
		return node;
	}

	void SetNode(Node* NewNode)
	{
		node = NewNode;
	}

	BuildingType GetBuildingType()
	{
		return buildingType;
	}

	float range = 0;

protected:
	Node* node = nullptr;
	BuildingType buildingType;
	bool isBuilt = false;

};