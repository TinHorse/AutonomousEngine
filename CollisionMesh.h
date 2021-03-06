#pragma once
#include "Components.h"
#include <unordered_map>

struct Collisionmesh
{
public:
	Collisionmesh() = default;

	void LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale);
	int operator()(const int& x, const int& y);

	const std::array<ColliderComponent*,9>& getRegion(const int& x, const int& y);
	void CalculateCollision();
	bool boundsCheck(const int& index);
	bool doesNodeExist(const int& index);
	ColliderComponent* getNodeAtPosition(const int& index);

	void addNode(ColliderComponent* comp);
	
	int cols, rows;
	int tileSizeX, tileSizeY;
	static int mesh_index;
	int index;

	std::vector<int> collision_mesh;
	std::unordered_map<int, ColliderComponent*> mesh_nodes;
	std::array<ColliderComponent*,9> mesh_neighbours;
};
