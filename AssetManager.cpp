#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"
#include "Entities.h"

extern Collisionmesh collision;

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
	manager->addTransformComponent(player, position.x, position.y, sizeX, sizeY, scale, true);

	auto& sprite = manager->addSpriteComponent(player, "player", false);
	//sprite.addAnimation("idle", 0, 10, 150);
	//sprite.addAnimation("walk", 1, 9, 50);
	//sprite.addAnimation("eat", 2, 6, 50);

	manager->addDynamicColliderComponent(player, "player", true, true);

	manager->addKeyboardController(player);
	manager->AddToGroup(&player, Game::groupPlayers);

	return player;
}

Entity& AssetManager::CreateHunted(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& hunted = manager->AddEntity<Hunted>(manager->GetGroup(Game::groupPlayers)[0]);
	manager->addTransformComponent(hunted, position.x, position.y, sizeX, sizeY, scale, true);
	auto& sprite = manager->addSpriteComponent(hunted, "player", false);
	//sprite.addAnimation("idle", 0, 10, 150);
	//sprite.addAnimation("walk", 1, 9, 50);
	//sprite.addAnimation("eat", 2, 6, 50);
	//sprite.addAnimation("dead", 3, 1, 50);


	manager->addDynamicColliderComponent(hunted,"hunted", true);
	manager->addPathfindingComponent(hunted);

	manager->AddToGroup(&hunted, Game::groupHunted);
	//aisystem.addAIEntity(&hunted, Game::groupHunted, manager->GetGroup(Game::groupPlayers)[0]);

	return hunted;
}

Entity & AssetManager::CreateFood(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& foodItem = manager->AddEntity<Grass>();
	auto& transform = manager->addTransformComponent(foodItem, position.x, position.y, sizeX, sizeY, scale, false);
	manager->addSpriteComponent(foodItem, "food", false);
	auto& col = manager->addDynamicColliderComponent(foodItem, "food", false);
	manager->AddToGroup(&foodItem, Game::groupFood);

	return foodItem;
}

Entity & AssetManager::CreatePredator(Vector2D position, int sizeX, int sizeY, float scale)
{
	auto& predator = manager->AddEntity<Predator>();
	manager->addTransformComponent(predator, position.x, position.y, sizeX, sizeY, scale, true);
	manager->addSpriteComponent(predator, "player", false);
	manager->addDynamicColliderComponent(predator, "predator", true);
	manager->addPathfindingComponent(predator);

	manager->AddToGroup(&predator, Game::groupPredators);

	return predator;
}

Entity & AssetManager::CreateCannonBall(Vector2D position, int angle, float speed, int sizeX, int sizeY, float scale)
{
	auto& ball = manager->AddEntity<CannonBall>();
	manager->addTransformComponent(ball, position.x, position.y, sizeX, sizeY, scale, true, angle, speed);
	manager->addDynamicColliderComponent(ball, "cannonBall", true, false);
	manager->addProjectileComponent(ball, position.x, position.y, scale, sizeX, sizeY, "player");

	manager->AddToGroup(&ball, Game::groupProjectiles);

	return ball;
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

