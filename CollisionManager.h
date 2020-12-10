#pragma once
#include "Collision.h"
#include "Components.h"
#include <vector>
#include <set>
using std::vector;

struct CollisionNode
{
	CollisionNode() {}
	CollisionNode(int X, int Y, bool obs) : x(X), y(Y), isObstacle(obs) {}
	int x, y;
	bool isObstacle;
	std::set<ColliderComponent*> colliders;
};

struct Matrix
{
public:
	Matrix() = default;

	void Init(int mCols, int mRows)
	{
		cols = mCols;
		rows = mRows;
		neighbours = std::vector<CollisionNode*>(9, nullptr);
	}
	CollisionNode* operator()(const int& x, const int& y)
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
	const std::vector<CollisionNode*>& getRegion(const int& x, const int& y)
	{
		neighbours[0] = (mesh[ y* cols + x]);
		if (boundsCheck((y)* cols + x + 1)) {
			neighbours[1] = (mesh[(y)* cols + x+1]);
		}
		if (boundsCheck((y + 1) * cols + x + 1)) {
			neighbours[2] = (mesh[(y + 1) * cols + x + 1]);
		}
		if (boundsCheck((y + 1) * cols + x)) {
			neighbours[3] = (mesh[(y + 1) * cols + x]);
		}
		if (boundsCheck((y + 1) * cols + x - 1)) {
			neighbours[4] = (mesh[(y + 1) * cols + x - 1]);
		}
		if (boundsCheck((y)* cols + x - 1)) {
			neighbours[5] = (mesh[(y)* cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x - 1)) {
			neighbours[6] = (mesh[(y - 1) * cols + x - 1]);
		}
		if (boundsCheck((y - 1) * cols + x)) {
			neighbours[7] = (mesh[(y - 1) * cols + x]);
		}
		if (boundsCheck((y - 1) * cols + x + 1)) {
			neighbours[8] = (mesh[(y - 1) * cols + x + 1]);
		}

		return neighbours;
	}

	bool boundsCheck(const int& index)
	{
		return (index >= 0 && index < mesh.size());
	}

	int cols, rows;
	std::vector<CollisionNode*> mesh;
	std::vector<CollisionNode*> neighbours;
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