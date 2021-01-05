#pragma once
#include "ECS.h"
#include "Components.h"
#include <set>

class EntityManager
{
public:
	void Update();
	void Refresh();
	EntityManager();

	void AddToGroup(Entity *entity, Group mGroup); // adds an entity to one of the groups
	Entity& AddEntity();
	std::vector<Entity*>& GetGroup(Group mGroup); // returns the specified group of entities

	
	std::vector<Entity*>& FindEntitiesInArea(const Vector2D& position, Group mGroup, const float& dist)
	{
		entities_in_area.clear();
		for (auto& e : GetGroup(mGroup))
		{
			if (Math::distance(e->GetComponent<TransformComponent>().position, position) < dist)
			{
				entities_in_area.push_back(e);
			}
		}
		return entities_in_area;
	}
	

	template<typename ...TArgs>
	TransformComponent& addTransformComponent(Entity& entity, TArgs && ...args)
	{
		compTran[index_tran] = TransformComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<TransformComponent>(&compTran[index_tran]);
		return compTran[index_tran++];
	}

	template<typename ...TArgs>
	SpriteComponent& addSpriteComponent(Entity& entity, TArgs && ...args)
	{
		compSprite[index_sprites] = SpriteComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<SpriteComponent>(&compSprite[index_sprites]);
		return compSprite[index_sprites++];
	}

	template<typename ...TArgs>
	KeyboardController& addKeyboardController(Entity& entity, TArgs && ...args)
	{
		compKeyboard[index_keyboard] = KeyboardController(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<KeyboardController>(&compKeyboard[index_keyboard]);
		return compKeyboard[index_keyboard++];
	}

	template<typename ...TArgs>
	ColliderComponent& addStaticColliderComponent(Entity& entity, TArgs && ...args)
	{
		compStaticColl[index_static_coll] = ColliderComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<ColliderComponent>(&compStaticColl[index_static_coll]);
		return compStaticColl[index_static_coll++];
	}

	template<typename ...TArgs>
	ColliderComponent& addDynamicColliderComponent(Entity& entity, TArgs && ...args)
	{
		compDynamicColl[index_dynamic_coll] = ColliderComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<ColliderComponent>(&compDynamicColl[index_dynamic_coll]);
		return compDynamicColl[index_dynamic_coll++];
	}

	template<typename ...TArgs>
	TileComponent& addTileComponent(Entity& entity, TArgs && ...args)
	{
		compTile[index_tiles] = TileComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<TileComponent>(&compTile[index_tiles]);
		return compTile[index_tiles++];
	}

	template<typename ...TArgs>
	PathfindingComponent& addPathfindingComponent(Entity& entity, TArgs && ...args)
	{
		compPath[index_path] = PathfindingComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<PathfindingComponent>(&compPath[index_path]);
		return compPath[index_path++];
	}	

	template<typename ...TArgs>
	StateComponent& addStateComponent(Entity& entity, TArgs && ...args)
	{
		compState[index_state] = StateComponent(std::forward<TArgs>(args)...);
		entity.AddColliderComponent<StateComponent>(&compState[index_state]);
		return compState[index_state++];
	}

	std::array<TileComponent, 2000>& getTiles()
	{
		return compTile;
	}

	std::array<TransformComponent, 2000> compTran;
	std::array<SpriteComponent, 2000> compSprite;
	std::array<KeyboardController, 2000> compKeyboard;
	std::array<ColliderComponent, 2000> compStaticColl;
	std::array<ColliderComponent, 2000> compDynamicColl;
	std::array<TileComponent, 2000> compTile;
	std::array<PathfindingComponent, 2000> compPath;
	std::array<StateComponent, 2000> compState;

	int index_tran = 0;
	int index_sprites = 0;
	int index_keyboard = 0;
	int index_static_coll = 0;
	int index_dynamic_coll = 0;
	int index_tiles = 0;
	int index_path = 0;
	int index_state = 0;

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; // same as entities, but grouped for rendering

	std::vector<Entity*> entities_in_area;

	std::set<Entity*> deleted_entities;
};

