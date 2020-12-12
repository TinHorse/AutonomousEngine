#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(EntityManager *man) : manager(man)
{
	
}
AssetManager::~AssetManager()
{
	
}

Entity& AssetManager::CreatePlayer(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& player = manager->AddEntity();
	player.AddComponent<TransformComponent>(position.x, position.y, sizeX, sizeY, scale);
	player.AddComponent<SpriteComponent>("player", true);
	player.AddComponent<ColliderComponent>("player");
	player.AddComponent<KeyboardController>();
	//player.AddComponent<AIControllerComponent>();
	player.AddToGroup(Game::groupPlayers);
	return player;
}

Entity& AssetManager::CreateAgent(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& agent = manager->AddEntity();
	agent.AddComponent<TransformComponent>(position.x, position.y, sizeX, sizeY, scale);
	agent.AddComponent<SpriteComponent>("player", false);
	agent.AddComponent<ColliderComponent>("agent");
	agent.AddComponent<PathfindingComponent>();
	agent.AddComponent<AIControllerComponent>();
	agent.AddToGroup(Game::groupAgents);
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

