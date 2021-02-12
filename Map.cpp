#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include "EntityManager.h"

extern EntityManager manager;

Map::Map(std::string tID, int mpTileSize, float mpScale) : texID(tID), tileSize(mpTileSize), mapScale(mpScale)
{
	scaledSize = mpTileSize * mpScale;
}

Map::~Map()
{
	
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.AddEntity());
	manager.addTileComponent(tile, srcX, srcY, xpos, ypos, mapScale, tileSize, texID);
	manager.AddToGroup(&tile, Game::groupTiles);
}


void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char t;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(t);
			srcY = atoi(&t) * tileSize;
			mapFile.get(t);
			srcX = atoi(&t) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.close();
}

