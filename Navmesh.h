#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <stack>
#include <map>
#include "ECS.h"

class Vector2D;

struct NavNode
{
	NavNode()
	{
		ID = ++NavNodeID;
	}
	NavNode(int X, int Y, bool obs) : x(X), y(Y), isObstacle(obs)
	{
		ID = ++NavNodeID;
	}
	bool operator==(const NavNode& NavNode)
	{
		return this->ID == NavNode.ID;
	}

	int x, y;
	int ID;
	bool isObstacle;
	long int globalDist = LONG_MAX;
	static int NavNodeID;
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

	NavNode &operator()(const int& x, const int& y);
	NavNode *getNavNodeAt(const int& x, const int& y);
	const std::vector<NavNode*>& getNeighbours(const int& x, const int& y);
	bool boundsCheck(const int& index);

private:
	int tileSizeX, tileSizeY;
	int cols, rows;

	// visited map
	std::map<int, bool> visited;

	// current parent NavNode map
	std::map<NavNode*, NavNode*> parents;

	// fLocal map
	std::map<NavNode*, long int> goals;

	std::vector<NavNode> mesh;
	std::vector<NavNode*> neighbours;

	NavNode nullNavNode;
};

struct NavNodeCompare
{
	bool operator()(const NavNode* nA, const NavNode* nB) const
	{
		return nA->globalDist > nB->globalDist;
	}
};