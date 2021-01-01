#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"

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
	manager->addDynamicColliderComponent(player, "player");
	manager->addKeyboardController(player);
	manager->AddToGroup(&player, Game::groupPlayers);
	return player;
}

Entity& AssetManager::CreateAgent(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& agent = manager->AddEntity();
	manager->addTransformComponent(agent, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(agent, "player", false);
	manager->addDynamicColliderComponent(agent,"agent");
	manager->addPathfindingComponent(agent);
	manager->addAIControllerComponent(agent);

	manager->AddToGroup(&agent, Game::groupAgents);
	return agent;
}

void AssetManager::AddTexture(std::string texID, const char *path)
{
	textures.emplace(texID, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string texID)
{
	return textures[texID];
}

