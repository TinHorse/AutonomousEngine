#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager *man) : manager(man)
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
	player.AddToGroup(Game::groupPlayers);
	return player;
}

void AssetManager::AddTexture(std::string texID, const char *path)
{
	textures.emplace(texID, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string texID)
{
	return textures[texID];
}

