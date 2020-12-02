#pragma once
#include "Matrix.h"
#include "Vector2D.h"
#include <stack>

struct Node
{
	Node(bool mIsObstacle, bool mVisited, float fGlob, float fLoc, int X, int Y)
	{
		isObstacle = mIsObstacle;
		visited = mVisited;
		fGlobal = fGlob;
		fLocal = fLoc;
		x = X;
		y = Y;
	}
	bool isObstacle = false;
	bool visited = false;
	float fGlobal;
	float fLocal;
	int x, y;
	Node *parent;
};

class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();

	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::stack<Vector2D> CalculatePath(Vector2D curLoc, Vector2D targetLoc);

private:
	Mat navMesh;
	Matrix<Node*, 0,0> matrix;
	int tileSizeX, tileSizeY;
};
