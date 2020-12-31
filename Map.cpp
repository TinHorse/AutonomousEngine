#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include "EntityManager.h"

extern EntityManager manager;

Map::Map(std::string tID, int mpTileSize, int mpScale) : texID(tID), tileSize(mpTileSize), mapScale(mpScale)
{
	scaledSize = mpTileSize * mpScale;
}

Map::~Map()
{
	
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.AddEntity());
	tile.AddComponent<TileComponent>(srcX, srcY, xpos, ypos, mapScale, tileSize, texID);
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
			srcY = atoi(&t) * 32;
			mapFile.get(t);
			srcX = atoi(&t) * 32;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(t);
			if (t == '1')
			{
				//auto& tileCol(manager.AddEntity());
				//tileCol.AddComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				//tileCol.AddToGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

