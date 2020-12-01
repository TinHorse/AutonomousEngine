#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	// Object Management
	Entity& CreatePlayer(Vector2D position, int sizeX = 32, int sizeY = 32, float scale = 1.f);

	// Texture Management
	void AddTexture(std::string texID, const char *path);
	SDL_Texture *GetTexture(std::string texID);

private:
	Manager *manager;
	std::map<std::string, SDL_Texture*> textures;
};