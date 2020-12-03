#pragma once
#include "NavMesh.h"
#include <stack>

class Vector2D;


class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();

	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::stack<Vector2D> CalculatePath(const Vector2D& curLoc, const Vector2D& targetLoc);

private:
	NavMesh navMesh;
	int tileSizeX, tileSizeY;
};
