#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"
#include "AISystem.h"

AssetManager::AssetManager(EntityManager *man) : manager(man)
{
	
}
AssetManager::~AssetManager()
{
	
}

Entity& AssetManager::CreatePlayer(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& player = manager->AddEntity();
	manager->addTransformComponent(player, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(player, "player", true);
	manager->addStaticColliderComponent(player, "player");
	manager->addKeyboardController(player);
	manager->AddToGroup(&player, Game::groupPlayers);
	return player;
}

Entity& AssetManager::CreateHunted(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& hunted = manager->AddEntity();
	manager->addTransformComponent(hunted, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(hunted, "player", false);
	manager->addDynamicColliderComponent(hunted,"agent");
	manager->addPathfindingComponent(hunted);
	manager->AddToGroup(&hunted, Game::groupHunted);

	return hunted;
}

Entity & AssetManager::CreateFood(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& foodItem = manager->AddEntity();
	manager->addTransformComponent(foodItem, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(foodItem, "food", false);
	manager->addDynamicColliderComponent(foodItem, "agent");
	manager->AddToGroup(&foodItem, Game::groupFood);

	return foodItem;
}

void AssetManager::AddTexture(std::string texID, const char *path)
{
	textures.emplace(texID, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string texID)
{
	return textures[texID];
}

