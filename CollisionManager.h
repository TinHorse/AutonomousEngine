#pragma once
#include "Collision.h"
#include "Components.h"
#include <vector>
#include <set>
using std::vector;


struct Matrix
{
public:
	Matrix() = default;

	void Init(int mCols, int mRows)
	{
		cols = mCols;
		rows = mRows;
		neighbours = std::vector<ColliderComponent*>(9);
	}
	int operator()(const int& x, const int& y)
	{
		if (boundsCheck(y * cols + x))
		{
			return mesh[y * cols + x];
		}
		else
		{
			return 0;
		}
	}
	const std::vector<ColliderComponent*>& getRegion(const int& x, const int& y)
	{
		if (DoesNodeExist((y)* cols + x)) {
			neighbours[0] = nodes[(mesh[y* cols + x])];
		}
		if (DoesNodeExist((y)* cols + x + 1)) {
			neighbours[1] = nodes[(mesh[(y)* cols + x+1])];
		}
		if (DoesNodeExist((y + 1) * cols + x + 1)) {
			neighbours[2] = nodes[(mesh[(y + 1) * cols + x + 1])];
		}
		if (DoesNodeExist((y + 1) * cols + x)) {
			neighbours[3] = nodes[(mesh[(y + 1) * cols + x])];
		}
		if (DoesNodeExist((y + 1) * cols + x - 1)) {
			neighbours[4] = nodes[(mesh[(y + 1) * cols + x - 1])];
		}
		if (DoesNodeExist((y)* cols + x - 1)) {
			neighbours[5] = nodes[(mesh[(y)* cols + x - 1])];
		}
		if (DoesNodeExist((y - 1) * cols + x - 1)) {
			neighbours[6] = nodes[(mesh[(y - 1) * cols + x - 1])];
		}
		if (DoesNodeExist((y - 1) * cols + x)) {
			neighbours[7] = nodes[(mesh[(y - 1) * cols + x])];
		}
		if (DoesNodeExist((y - 1) * cols + x + 1)) {
			neighbours[8] = nodes[(mesh[(y - 1) * cols + x + 1])];
		}

		return neighbours;
	}

	bool boundsCheck(const int& index)
	{
		return (index >= 0 && index < mesh.size());
	}
	bool DoesNodeExist(const int& index)
	{
		return (index >= 0 && index < mesh.size() && mesh[index] != 0);
	}

	void addNode(ColliderComponent* comp)
	{
		nodes.insert(std::make_pair(matIndex++, comp));
	}

	int cols, rows;
	static int matIndex;
	int index;
	std::vector<int> mesh;
	std::map<int, ColliderComponent*> nodes;
	std::vector<ColliderComponent*> neighbours;
};

class CollisionManager
{
public:
	void LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale);
	void AddAgent(const Entity& agent);
	void CalculateCollision();


private:
	Matrix colMat;
	vector<ColliderComponent*> dynamic_colliders;
	int tileSizeX, tileSizeY;
};