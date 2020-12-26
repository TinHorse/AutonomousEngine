#pragma once
#include <vector>
#include <iostream>
#include <stack>
#include <map>

class Vector2D;

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
	static int NodeID;
};

struct NavMesh
{
public:
	NavMesh();
	~NavMesh();

	void Init(int mCols, int mRows);
	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::stack<Vector2D> CalculatePath(Vector2D curLoc, Vector2D targetLoc);
	void ClearMesh();

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

	Node *getNodeAt(const int& x, const int& y)
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
private:
	int tileSizeX, tileSizeY;

	// visited map
	std::map<int, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;

	// fLocal and fGlobal map
	std::map<Node*, int> goals;

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