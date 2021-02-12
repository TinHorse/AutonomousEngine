#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <stack>
#include <map>
#include "ECS.h"

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
	long int globalDist = LONG_MAX;
	static int NodeID;
};

struct Navmesh
{
public:
	Navmesh();
	~Navmesh();

	void Init(int mCols, int mRows);
	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, float scale);
	bool CalculatePath(Entity* entity, std::stack<Vector2D>& path, const Vector2D targetLoc);
	void ClearMesh();

	Node &operator()(const int& x, const int& y);
	Node *getNodeAt(const int& x, const int& y);
	const std::vector<Node*>& getNeighbours(const int& x, const int& y);
	bool boundsCheck(const int& index);

private:
	int tileSizeX, tileSizeY;
	int cols, rows;

	// visited map
	std::map<int, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;

	// fLocal map
	std::map<Node*, long int> goals;

	std::vector<Node> mesh;
	std::vector<Node*> neighbours;

	Node nullNode;
};

struct NodeCompare
{
	bool operator()(const Node* nA, const Node* nB) const
	{
		return nA->globalDist > nB->globalDist;
	}
};