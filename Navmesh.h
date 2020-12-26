#pragma once
#include <vector>
#include <iostream>
#include <stack>
#include <unordered_map>

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

struct Navmesh
{
public:
	Navmesh();
	~Navmesh();

	void Init(int mCols, int mRows);
	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::stack<Vector2D> CalculatePath(Vector2D curLoc, Vector2D targetLoc);
	void ClearMesh();

	Node *operator()(const int& x, const int& y);
	Node *getNodeAt(const int& x, const int& y);
	const std::vector<Node*>& getNeighbours(const int& x, const int& y);
	bool boundsCheck(const int& index);
private:
	int tileSizeX, tileSizeY;
	int cols, rows;

	// visited map
	std::unordered_map<int, bool> visited;

	// current parent node map
	std::unordered_map<Node*, Node*> parents;

	// fLocal and fGlobal map
	std::unordered_map<Node*, int> goals;

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