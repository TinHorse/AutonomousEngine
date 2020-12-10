#pragma once
#include "NavMesh.h"
#include <stack>
#include <map>

class Vector2D;

class NavigationManager
{
public:
	NavigationManager();
	~NavigationManager();
	void Init();

	void LoadMesh(const char *path, int sX, int sY, int sTileX, int sTileY, int scale);
	std::stack<Vector2D> CalculatePath(Vector2D curLoc, Vector2D targetLoc);

	void Restart();

private:
	NavMesh navMesh;
	int tileSizeX, tileSizeY;

	// visited map
	std::map<int, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;

	// fLocal and fGlobal map
	std::map<Node*, int> goals;
};
