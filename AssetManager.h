#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"

class AssetManager
{
public:
	AssetManager(EntityManager* man);
	~AssetManager();

	// Object Management
	Entity& CreatePlayer(Vector2D position, int sizeX = 32, int sizeY = 32, float scale = 1.f);
	Entity& CreateHunted(Vector2D position, int sizeX = 32, int sizeY = 32, float scale = 1.f);
	Entity& CreateFood(Vector2D position, int sizeX = 32, int sizeY = 32, float scale = 1.f);

	// Texture Management
	void AddTexture(std::string texID, const char *path);
	SDL_Texture *GetTexture(std::string texID);
	

private:
	EntityManager *manager;
	std::map<std::string, SDL_Texture*> textures;

	std::vector<ColliderComponent> colliderComponents = std::vector<ColliderComponent>(1000);
	int collIndex = 0;
};