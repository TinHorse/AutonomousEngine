#pragma once
#include "Matrix.h"
#include "Vector2D.h"
#include <queue>

class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();

	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::queue<Vector2D> CalculatePath(Vector2D curLoc, Vector2D targetLoc);

private:
	Mat navMesh;
	int tileSizeX, tileSizeY;
};
