#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"
#include "AISystem.h"

AssetManager::AssetManager(EntityManager *man) : manager(man)
{
	
}
AssetManager::~AssetManager()
{
	for (auto& t : textures)
	{
		SDL_DestroyTexture(t.second);
	}
}

Entity& AssetManager::CreatePlayer(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& player = manager->AddEntity();
	manager->addTransformComponent(player, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(player, "player", true);
	manager->addStaticColliderComponent(player, "player");
	manager->addKeyboardController(player);
	manager->AddToGroup(&player, Game::groupPlayers);

	auto& state = manager->addStateComponent(player);
	state.initS("calm", 100);

	return player;
}

Entity& AssetManager::CreateHunted(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& hunted = manager->AddEntity();
	manager->addTransformComponent(hunted, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(hunted, "enemy", false);
	manager->addDynamicColliderComponent(hunted,"agent");
	auto& pathfinder = manager->addPathfindingComponent(hunted);

	manager->AddToGroup(&hunted, Game::groupHunted);

	auto& state = manager->addStateComponent(hunted);
	state.initS("health", 100);
	state.initS("hunger", 50);
	state.initS("calm", 0);
	state.initS("food", 0);

	state.initB("exploring", 100);
	state.initB("returningToShepherd", 100);
	state.initB("fleeing", 0);

	//state.pushBehaviour(exploring, 0);

	pathfinder.initTarget("current", nullptr);
	pathfinder.initTarget("origin", manager->GetGroup(Game::groupPlayers)[0]);

	return hunted;
}

Entity & AssetManager::CreateFood(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& foodItem = manager->AddEntity();
	manager->addTransformComponent(foodItem, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(foodItem, "food", false);
	manager->addDynamicColliderComponent(foodItem, "food");
	manager->AddToGroup(&foodItem, Game::groupFood);

	auto& state = manager->addStateComponent(foodItem);
	state.initS("food", 10);

	return foodItem;
}

void AssetManager::AddTexture(std::string texID, const char *path)
{
	assert(textures.find(texID) == textures.end());
	textures[texID] = TextureManager::LoadTexture(path);
}

SDL_Texture *AssetManager::GetTexture(std::string texID)
{
	return textures[texID];
}

