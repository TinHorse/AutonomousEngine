#pragma once
#include "Components.h"

struct CollisionMesh
{
public:
	CollisionMesh() = default;

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
			neighbours[0] = GetNodeAtPosition(y* cols + x);
			neighbours[1] = GetNodeAtPosition(y* cols + x + 1);
			neighbours[2] = GetNodeAtPosition((y + 1) * cols + x + 1);
			neighbours[3] = GetNodeAtPosition((y + 1) * cols + x);
			neighbours[4] = GetNodeAtPosition((y + 1) * cols + x - 1);
			neighbours[5] = GetNodeAtPosition(y* cols + x - 1);
			neighbours[6] = GetNodeAtPosition((y - 1) * cols + x - 1);
			neighbours[7] = GetNodeAtPosition((y - 1) * cols + x);
			neighbours[8] = GetNodeAtPosition((y - 1) * cols + x + 1);

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
	ColliderComponent* GetNodeAtPosition(const int& index)
	{
		if (index >= 0 && index < mesh.size() && mesh[index] != 0)
		{
			return nodes[(mesh[index])];
		}
		return nullptr;
	}

	void addNode(ColliderComponent* comp)
	{
		nodes.insert(std::make_pair(++matIndex, comp));
	}
	
	int cols, rows;
	static int matIndex;
	int index;
	std::vector<int> mesh;
	std::map<int, ColliderComponent*> nodes;
	std::vector<ColliderComponent*> neighbours;
};