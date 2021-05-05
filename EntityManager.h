#pragma once
#include "ECS.h"
#include "Components.h"
#include <set>
#include <cassert>

class EntityManager
{
public:
	void Update();
	void Refresh();
	EntityManager();

	void AddToGroup(Entity *entity, Group mGroup); // adds an entity to one of the groups
	template<typename T = Entity, typename ...TArgs>
	T & AddEntity(TArgs &&...args)
	{
		T *entity = new T(*this, std::forward<TArgs>(args)...);

		std::unique_ptr<Entity> uPtr{ entity }; // creates a new unique_ptr that points to the entity. Note that assignment from standard pointer to unique pointer cannot be done through uPtr = entity but must instead be done via {} operator

		entities.emplace_back(std::move(uPtr)); // "emplace_back" gets rid of having to do an unnecessary copy of the element, instead just using the data that is already there at the same memory location. Using "std::move" in conjunction with "emplace_back" is only useful if the element already exists, which is true in this case
		return *entity;
	}

	std::vector<Entity*>& GetGroup(Group mGroup); // returns the specified group of entities
	
	std::vector<Entity*>& FindEntitiesInArea(const Vector2D& position, Group mGroup, const float& dist);

	template<typename T>
	std::vector<T*> FindEntitiesInAreaByType(const Vector2D& position, Group mGroup, const float& dist)
	{
		std::vector<T*> typedEntities;
		for (Entity* entity : GetGroup(mGroup))
		{
			if (Collision::CircularCollision(entity->GetComponent<TransformComponent>().position, position, dist))
			{
				T* t = dynamic_cast<T*>(entity);
				if (t)
				{
					typedEntities.push_back(t);
				}
			}
		}
		return typedEntities;
	}

	Entity* FindOneEntityInArea(const Vector2D& position, const Group& mGroup, const float& dist);
	
	std::set<Entity*>* getDeletedEntities();

	template<typename ...TArgs>
	TransformComponent& addTransformComponent(Entity& entity, TArgs && ...args)
	{
		assert(index_tran < 5000 && "index overflow");
		compTran[index_tran] = TransformComponent(std::forward<TArgs>(args)...);
		entity.addComponent<TransformComponent>(&compTran[index_tran]);
		return compTran[index_tran++];
	}

	template<typename ...TArgs>
	SpriteComponent& addSpriteComponent(Entity& entity, TArgs && ...args)
	{
		assert(index_tran < 5000 && "index overflow");
		compSprite[index_sprites] = SpriteComponent(std::forward<TArgs>(args)...);
		entity.addComponent<SpriteComponent>(&compSprite[index_sprites]);
		return compSprite[index_sprites++];
	}

	template<typename ...TArgs>
	KeyboardController& addKeyboardController(Entity& entity, TArgs && ...args)
	{
		compKeyboard[index_keyboard] = KeyboardController(std::forward<TArgs>(args)...);
		entity.addComponent<KeyboardController>(&compKeyboard[index_keyboard]);
		return compKeyboard[index_keyboard++];
	}

	template<typename ...TArgs>
	ColliderComponent& addDynamicColliderComponent(Entity& entity, TArgs && ...args)
	{
		compDynamicColl[index_dynamic_coll] = ColliderComponent(std::forward<TArgs>(args)...);
		entity.addComponent<ColliderComponent>(&compDynamicColl[index_dynamic_coll]);
		return compDynamicColl[index_dynamic_coll++];
	}

	template<typename ...TArgs>
	PathfindingComponent& addPathfindingComponent(Entity& entity, TArgs && ...args)
	{
		compPath[index_path] = PathfindingComponent(std::forward<TArgs>(args)...);
		entity.addComponent<PathfindingComponent>(&compPath[index_path]);
		return compPath[index_path++];
	}	

	template<typename ...TArgs>
	ProjectileComponent& addProjectileComponent(Entity& entity, TArgs && ...args)
	{
		compProjectile[index_proj] = ProjectileComponent(std::forward<TArgs>(args)...);
		entity.addComponent<ProjectileComponent>(&compProjectile[index_proj]);
		return compProjectile[index_proj++];
	}

	template<typename ...TArgs>
	StorageComponent& addStorageComponent(Entity& entity, TArgs && ...args)
	{
		compStorage[index_storage] = StorageComponent(std::forward<TArgs>(args)...);
		entity.addComponent<StorageComponent>(&compStorage[index_storage]);
		return compStorage[index_storage++];
	}

	template<typename ...TArgs>
	NodeMovementComponent& addNodeMovementComponent(Entity& entity, TArgs && ...args)
	{
		compNodeMov[index_node_mov] = NodeMovementComponent(std::forward<TArgs>(args)...);
		entity.addComponent<NodeMovementComponent>(&compNodeMov[index_node_mov]);
		return compNodeMov[index_node_mov++];
	}

	template<typename ...TArgs>
	ProductionComponent& addProductionComponent(Entity& entity, TArgs && ...args)
	{
		compProd[index_prod] = ProductionComponent(std::forward<TArgs>(args)...);
		entity.addComponent<ProductionComponent>(&compProd[index_prod]);
		return compProd[index_prod++];
	}

	template<typename ...TArgs>
	PlayerResourceComponent& addPlayerResourceComponent(Entity& entity, TArgs && ...args)
	{
		compPlayerRes[index_player_res] = PlayerResourceComponent(std::forward<TArgs>(args)...);
		entity.addComponent<PlayerResourceComponent>(&compPlayerRes[index_player_res]);
		return compPlayerRes[index_player_res++];
	}

	template<typename ...TArgs>
	ManufacturingComponent& addManufacturingComponent(Entity& entity, TArgs && ...args)
	{
		compManufac[index_manufac] = ManufacturingComponent(std::forward<TArgs>(args)...);
		entity.addComponent<ManufacturingComponent>(&compManufac[index_manufac]);
		return compManufac[index_manufac++];
	}

	template<typename ...TArgs>
	HangarComponent& addHangarComponent(Entity& entity, TArgs && ...args)
	{
		compHangar[index_hangar] = HangarComponent(std::forward<TArgs>(args)...);
		entity.addComponent<HangarComponent>(&compHangar[index_hangar]);
		return compHangar[index_hangar++];
	}

private:
	std::array<TransformComponent, 5000> compTran;
	std::array<SpriteComponent, 5000> compSprite;
	std::array<KeyboardController, 1> compKeyboard;
	std::array<ColliderComponent, 5000> compDynamicColl;
	std::array<PathfindingComponent, 5000> compPath;
	std::array<ProjectileComponent, 5000> compProjectile;
	std::array<StorageComponent, 5000> compStorage;
	std::array<NodeMovementComponent, 5000> compNodeMov;
	std::array<ProductionComponent, 5000> compProd;
	std::array<PlayerResourceComponent, 1> compPlayerRes;
	std::array<ManufacturingComponent, 5000> compManufac;
	std::array<HangarComponent, 5000> compHangar;

	int index_tran = 0;
	int index_sprites = 0;
	int index_keyboard = 0;
	int index_dynamic_coll = 0;
	int index_path = 0;
	int index_proj = 0;
	int index_storage = 0;
	int index_node_mov = 0;
	int index_prod = 0;
	int index_player_res = 0;
	int index_manufac = 0;
	int index_hangar = 0;

	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; // same as entities, but grouped for rendering


	std::vector<Entity*> entities_in_area;

	std::set<Entity*> deleted_entities;
};

