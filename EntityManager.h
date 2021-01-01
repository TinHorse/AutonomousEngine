#pragma once
#include "ECS.h"
#include "Components.h"

class EntityManager
{
public:
	void Update();
	void Refresh();

	void AddToGroup(Entity *entity, Group mGroup); // adds an entity to one of the groups
	Entity& AddEntity();
	std::vector<Entity*>& GetGroup(Group mGroup); // returns the specified group of entities

	template<typename ...TArgs>
	void addTransformComponent(Entity& entity, TArgs ...args)
	{
		compTran[index_tran] = TransformComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<TransformComponent>(&compTran[index_tran]);
		compTran[index_tran].Init();
		index_tran++;
	}

	template<typename ...TArgs>
	void addSpriteComponent(Entity& entity, TArgs ...args)
	{
		compSprite[index_sprites] = SpriteComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<SpriteComponent>(&compSprite[index_sprites]);
		compSprite[index_sprites].Init();
		index_sprites++;
	}

	template<typename ...TArgs>
	void addKeyboardController(Entity& entity, TArgs ...args)
	{
		compKeyboard[index_keyboard] = KeyboardController(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<KeyboardController>(&compKeyboard[index_keyboard]);
		compKeyboard[index_keyboard].Init();
		index_keyboard++;
	}

	template<typename ...TArgs>
	void addStaticColliderComponent(Entity& entity, TArgs ...args)
	{
		compStaticColl[index_static_coll] = ColliderComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<ColliderComponent>(&compStaticColl[index_static_coll]);
		compStaticColl[index_static_coll].Init();
		index_static_coll++;
	}

	template<typename ...TArgs>
	void addDynamicColliderComponent(Entity& entity, TArgs ...args)
	{
		compDynamicColl[index_dynamic_coll] = ColliderComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<ColliderComponent>(&compDynamicColl[index_dynamic_coll]);
		compDynamicColl[index_dynamic_coll].Init();
		index_dynamic_coll++;
	}

	template<typename ...TArgs>
	void addTileComponent(Entity& entity, TArgs ...args)
	{
		compTile[index_tiles] = TileComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<TileComponent>(&compTile[index_tiles]);
		compTile[index_tiles].Init();
		index_tiles++;
	}

	template<typename ...TArgs>
	void addPathfindingComponent(Entity& entity, TArgs ...args)
	{
		compPath[index_path] = PathfindingComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<PathfindingComponent>(&compPath[index_path]);
		compPath[index_path].Init();
		index_path++;
	}

	template<typename ...TArgs>
	void addAIControllerComponent(Entity& entity, TArgs ...args)
	{
		compAI[index_ai] = AIControllerComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<AIControllerComponent>(&compAI[index_ai]);
		compAI[index_ai].Init();
		index_ai++;
	}
	

	std::array<TransformComponent, 2000> compTran;
	std::array<SpriteComponent, 2000> compSprite;
	std::array<KeyboardController, 2000> compKeyboard;
	std::array<ColliderComponent, 2000> compStaticColl;
	std::array<ColliderComponent, 2000> compDynamicColl;
	std::array<TileComponent, 2000> compTile;
	std::array<PathfindingComponent, 2000> compPath;
	std::array<AIControllerComponent, 2000> compAI;

	int index_tran = 0;
	int index_sprites = 0;
	int index_keyboard = 0;
	int index_static_coll = 0;
	int index_dynamic_coll = 0;
	int index_tiles = 0;
	int index_path = 0;
	int index_ai = 0;

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; // same as entities, but grouped for rendering
};

