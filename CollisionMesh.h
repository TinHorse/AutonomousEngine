#pragma once
#include "Components.h"

struct Collisionmesh
{
public:
	Collisionmesh() = default;

	void LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale);
	int operator()(const int& x, const int& y);

	const std::vector<ColliderComponent*>& getRegion(const int& x, const int& y);
	void CalculateCollision();
	bool boundsCheck(const int& index);
	bool doesNodeExist(const int& index);
	ColliderComponent* getNodeAtPosition(const int& index);

	void addNode(ColliderComponent* comp);
	
	int cols, rows;
	int tileSizeX, tileSizeY;
	static int matIndex;
	int index;
	std::vector<int> mesh;
	std::map<int, ColliderComponent*> nodes;
	std::vector<ColliderComponent*> neighbours;
};
