#include "NavigationManager.h"
#include <fstream>

NavigationManager::NavigationManager()
{
	
}

NavigationManager::~NavigationManager()
{
}

void NavigationManager::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	navMesh.mat = std::vector<int>(sX*sY);
	tileSizeX = sTileX * scale;
	tileSizeY = sTileY * scale;

	char c;
	std::fstream stream;
	stream.open(path);
	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sY; ++x)
		{
			stream.get(c);
			if (c == '1')
			{
				navMesh(x, y) = atoi(&c);
			}
			stream.ignore();
		}
	}
	stream.close();
}

std::queue<Vector2D> NavigationManager::CalculatePath(Vector2D curLoc, Vector2D targetLoc)
{
	std::queue<Vector2D> path;
	path.push(curLoc);
	int closestX = (curLoc.x / tileSizeX);
	int closestY = (curLoc.y / tileSizeY);

	path.push(Vector2D(closestX * tileSizeX, closestY * tileSizeY));

	int start = navMesh(closestX, closestY);

	path.push(targetLoc);
	return path;
}

