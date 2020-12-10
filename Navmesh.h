#pragma once
#include <vector>
#include <iostream>

static int NodeID = 0;
struct Node
{
	Node()
	{
		ID = ++NodeID;
	}
	Node(int X, int Y, bool obs) : x(X), y(Y), isObstacle(obs)
	{
		ID = ++NodeID;
	}
	bool operator==(const Node& node)
	{
		return this->ID == node.ID;
	}

	int x, y;
	int ID;
	bool isObstacle;
	int globalDist = INT_MAX;
};

struct NavMesh
{
public:
	NavMesh() = default;

	void Init(int mCols, int mRows)
	{
		cols = mCols;
		rows = mRows;
		neighbours = std::vector<Node*>(8);
	}
	Node* operator()(const int& x, const int& y)
	{
		if (boundsCheck(y * cols + x))
		{
			return mesh[y * cols + x];
		}
		else
		{
			return nullptr;
		}
	}
	const std::vector<Node*>& getNeighbours(const int& x, const int& y)
	{
		if (boundsCheck((y) * cols + x + 1)) {
			neighbours[0] = (mesh[(y)* cols + x + 1]);
		}
		if (boundsCheck((y + 1) * cols + x + 1)) {
			neighbours[1] = (mesh[(y + 1) * cols + x + 1]);
		}
		if (boundsCheck((y + 1) * cols + x)) {
			neighbours[2] = (mesh[(y + 1) * cols + x]);
		}
		if (boundsCheck((y + 1) * cols + x - 1)) {
			neighbours[3] = (mesh[(y + 1) * cols + x - 1]);
		}
		if (boundsCheck((y) * cols + x - 1)) {
			neighbours[4] = (mesh[(y)* cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x - 1)) {
			neighbours[5] = (mesh[(y - 1) * cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x)) {
			neighbours[6] = (mesh[(y - 1) * cols + x]);
		}
		if (boundsCheck((y - 1) * cols + x + 1)) {
			neighbours[7] = (mesh[(y - 1) * cols + x + 1]);
		}

		return neighbours;
	}
	
	bool boundsCheck(const int& index)
	{
		return (index >= 0 && index < mesh.size());
	}

	int cols, rows;
	std::vector<Node*> mesh;
	std::vector<Node*> neighbours;
};

struct NodeCompare
{
	bool operator()(const Node* nA, const Node* nB) const
	{
		return nA->globalDist > nB->globalDist;
	}

};