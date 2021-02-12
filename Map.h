#pragma once
#include <string>

class Map
{
public:
	Map(std::string tID, int mpTileSize, float mpScale);
	~Map();

	void AddTile(int srcX, int srcY, int xpos, int ypos);
	void LoadMap(std::string path, int sizeX, int sizeY);

private:
	std::string texID;
	float mapScale;
	int tileSize;
	int scaledSize;
};