#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"
#include "Entities.h"

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
	Entity& player = manager->AddEntity<Player>();
	manager->addTransformComponent(player, position.x, position.y, sizeX, sizeY, scale);

	auto& sprite = manager->addSpriteComponent(player, "player", true);
	sprite.addAnimation("idle", 0, 10, 150);
	sprite.addAnimation("walk", 1, 9, 50);
	sprite.addAnimation("eat", 2, 6, 50);

	manager->addStaticColliderComponent(player, "player");
	manager->addKeyboardController(player);
	manager->AddToGroup(&player, Game::groupPlayers);

	auto& state = manager->addStateComponent(player, "player");
	state.initS("calm", 100);

	return player;
}

Entity& AssetManager::CreateHunted(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& hunted = manager->AddEntity<Hunted>(manager->GetGroup(Game::groupPlayers)[0]);
	manager->addTransformComponent(hunted, position.x, position.y, sizeX, sizeY, scale);
	auto& sprite = manager->addSpriteComponent(hunted, "player", true);
	sprite.addAnimation("idle", 0, 10, 150);
	sprite.addAnimation("walk", 1, 9, 50);
	sprite.addAnimation("eat", 2, 6, 50);


	manager->addDynamicColliderComponent(hunted,"hunted");
	manager->addPathfindingComponent(hunted);

	manager->AddToGroup(&hunted, Game::groupHunted);
	//aisystem.addAIEntity(&hunted, Game::groupHunted, manager->GetGroup(Game::groupPlayers)[0]);

	return hunted;
}

Entity & AssetManager::CreateFood(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& foodItem = manager->AddEntity<Grass>();
	manager->addTransformComponent(foodItem, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(foodItem, "food", false);
	manager->addDynamicColliderComponent(foodItem, "food");
	manager->AddToGroup(&foodItem, Game::groupFood);

	auto& state = manager->addStateComponent(foodItem, "food");
	state.initS("food", 10);

	return foodItem;
}

Entity & AssetManager::CreatePredator(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& predator = manager->AddEntity<Predator>();
	manager->addTransformComponent(predator, position.x, position.y, sizeX, sizeY, scale);
	manager->addSpriteComponent(predator, "enemy2", false);
	manager->addDynamicColliderComponent(predator, "predator");
	auto& pathfinder = manager->addPathfindingComponent(predator);

	manager->AddToGroup(&predator, Game::groupPredators);

	auto& state = manager->addStateComponent(predator, "predator");
	state.initS("health", 100);
	state.initS("hunger", 50);
	state.initS("carrion", 0);

	state.initB("exploring", 100);
	state.initB("attacking", 100);
	state.initB("fleeing", 0);

	state.initTarget("current", nullptr);
	state.initTarget("origin", nullptr);

	return predator;
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

