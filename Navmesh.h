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
	bool operator==(Node& node)
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
	std::vector<Node*> getNeighbours(const int& x, const int& y)
	{
		std::vector<Node*> neighbours;
		if (boundsCheck((y) * cols + x + 1)) {
			neighbours.push_back(mesh[(y)* cols + x]);
		}
		if (boundsCheck((y + 1) * cols + x + 1)) {
			neighbours.push_back(mesh[(y + 1) * cols + x + 1]);
		}
		if (boundsCheck((y + 1) * cols + x)) {
			neighbours.push_back(mesh[(y + 1) * cols + x]);
		}
		if (boundsCheck((y + 1) * cols + x - 1)) {
			neighbours.push_back(mesh[(y + 1) * cols + x - 1]);
		}
		if (boundsCheck((y) * cols + x - 1)) {
			neighbours.push_back(mesh[(y)* cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x - 1)) {
			neighbours.push_back(mesh[(y - 1) * cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x)) {
			neighbours.push_back(mesh[(y - 1) * cols + x]);
		}
		if (boundsCheck((y - 1) * cols + x + 1)) {
			neighbours.push_back(mesh[(y - 1) * cols + x + 1]);
		}

		return neighbours;
	}
	
	bool boundsCheck(const int& index)
	{
		return (index >= 0 && index < mesh.size());
	}

	int cols, rows;
	std::vector<Node*> mesh;
};

struct NodeCompare
{
	bool operator()(const Node* nA, const Node* nB) const
	{
		return nA->globalDist > nB->globalDist;
	}

};