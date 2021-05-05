#include "EntityManager.h"
#include "Collision.h"

void EntityManager::Update()
{
	int index;

	index = index_tran;
	for (auto& comp : compTran)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_sprites;
	for (auto& comp : compSprite)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_keyboard;
	for (auto& comp : compKeyboard)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_dynamic_coll;
	for (auto& comp : compDynamicColl)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_path;
	for (auto& comp : compPath)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_proj;
	for (auto& comp : compProjectile)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_storage;
	for (auto& comp : compStorage)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_node_mov;
	for (auto& comp : compNodeMov)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_prod;
	for (auto& comp : compProd)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}
	index = index_player_res;
	for (auto& comp : compPlayerRes)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_manufac;
	for (auto& comp : compManufac)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}

	index = index_hangar;
	for (auto& comp : compHangar)
	{
		if (index-- <= 0) { break; };
		comp.Update();
	}
}

void EntityManager::Refresh()
{
	for (auto& e : GetGroup(Game::groupResources))
	{
		e->Update();
	}

	deleted_entities.clear();
	for (auto& e : entities)
	{
		if (!e.get()->IsActive())
		{
			deleted_entities.insert(e.get());
		}
	}

	if (deleted_entities.size() > 0)
	{

		// refresh agents
		// NOTE: Players dont need to refresh (update pointers to entities), but left this here to show hwere it should go
		for (auto* e : GetGroup(Game::groupPlayers))
		{
			e->refresh();
		}

		// remove inactive components
		int index = 0;
		for (auto& comp : compTran)
		{
			if (index >= index_tran) { break; };
			if (!comp.IsActive())
			{
				while (index_tran > 0 && !compTran[index_tran - 1].IsActive())
				{
					index_tran--;
				}
				if (index_tran > 0 && index_tran != index)
				{
					Entity* entity = compTran[index_tran - 1].entity;
					std::swap(compTran[index], compTran[index_tran - 1]);
					entity->SetComponent<TransformComponent>(&compTran[index]);
					index_tran--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compSprite)
		{
			if (index >= index_sprites) { break; };
			if (!comp.IsActive())
			{
				while (index_sprites > 0 && !compSprite[index_sprites - 1].IsActive())
				{
					index_sprites--;
				}
				if (index_sprites > 0 && index_sprites != index)
				{
					Entity* entity = compSprite[index_sprites - 1].entity;
					std::swap(compSprite[index], compSprite[index_sprites - 1]);
					entity->SetComponent<SpriteComponent>(&compSprite[index]);
					index_sprites--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compDynamicColl)
		{
			if (index >= index_dynamic_coll) { break; };
			if (!comp.IsActive())
			{
				while (index_dynamic_coll > 0 && !compDynamicColl[index_dynamic_coll - 1].IsActive())
				{
					index_dynamic_coll--;
				}
				if (index_dynamic_coll > 0 && index_dynamic_coll != index)
				{
					Entity* entity = compDynamicColl[index_dynamic_coll - 1].entity;
					std::swap(compDynamicColl[index], compDynamicColl[index_dynamic_coll - 1]);
					entity->SetComponent<ColliderComponent>(&compDynamicColl[index]);
					index_dynamic_coll--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compPath)
		{
			if (index >= index_path) { break; };
			if (!comp.IsActive())
			{
				while (index_path > 0 && !compPath[index_path - 1].IsActive())
				{
					index_path--;
				}
				if (index_path > 0 && index_path != index)
				{
					Entity* entity = compPath[index_path - 1].entity;
					std::swap(compPath[index], compPath[index_path - 1]);
					entity->SetComponent<PathfindingComponent>(&compPath[index]);
					index_path--;
				}
			}
			index++;
		}
		index = 0;
		for (auto& comp : compProjectile)
		{
			if (index >= index_proj) { break; };
			if (!comp.IsActive())
			{
				while (index_proj > 0 && !compProjectile[index_proj - 1].IsActive())
				{
					index_proj--;
				}
				if (index_proj > 0 && index_proj != index)
				{
					Entity* entity = compProjectile[index_proj - 1].entity;
					std::swap(compProjectile[index], compProjectile[index_proj - 1]);
					entity->SetComponent<ProjectileComponent>(&compProjectile[index]);
					index_proj--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compStorage)
		{
			if (index >= index_storage) { break; };
			if (!comp.IsActive())
			{
				while (index_storage > 0 && !compStorage[index_storage - 1].IsActive())
				{
					index_storage--;
				}
				if (index_storage > 0 && index_storage != index)
				{
					Entity* entity = compStorage[index_storage - 1].entity;
					std::swap(compStorage[index], compStorage[index_storage - 1]);
					entity->SetComponent<StorageComponent>(&compStorage[index]);
					index_storage--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compNodeMov)
		{
			if (index >= index_node_mov) { break; };
			if (!comp.IsActive())
			{
				while (index_node_mov > 0 && !compNodeMov[index_node_mov - 1].IsActive())
				{
					index_node_mov--;
				}
				if (index_node_mov > 0 && index_node_mov != index)
				{
					Entity* entity = compNodeMov[index_node_mov - 1].entity;
					std::swap(compNodeMov[index], compNodeMov[index_node_mov - 1]);
					entity->SetComponent<NodeMovementComponent>(&compNodeMov[index]);
					index_node_mov--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compProd)
		{
			if (index >= index_prod) { break; };
			if (!comp.IsActive())
			{
				while (index_prod > 0 && !compProd[index_prod - 1].IsActive())
				{
					index_prod--;
				}
				if (index_prod > 0 && index_prod != index)
				{
					Entity* entity = compProd[index_prod - 1].entity;
					std::swap(compProd[index], compProd[index_prod - 1]);
					entity->SetComponent<ProductionComponent>(&compProd[index]);
					index_prod--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compManufac)
		{
			if (index >= index_manufac) { break; };
			if (!comp.IsActive())
			{
				while (index_manufac > 0 && !compManufac[index_manufac - 1].IsActive())
				{
					index_manufac--;
				}
				if (index_manufac > 0 && index_manufac != index)
				{
					Entity* entity = compManufac[index_manufac - 1].entity;
					std::swap(compManufac[index], compManufac[index_manufac - 1]);
					entity->SetComponent<ManufacturingComponent>(&compManufac[index]);
					index_manufac--;
				}
			}
			index++;
		}

		index = 0;
		for (auto& comp : compHangar)
		{
			if (index >= index_hangar) { break; };
			if (!comp.IsActive())
			{
				while (index_hangar > 0 && !compHangar[index_hangar - 1].IsActive())
				{
					index_hangar--;
				}
				if (index_hangar > 0 && index_hangar != index)
				{
					Entity* entity = compHangar[index_hangar - 1].entity;
					std::swap(compHangar[index], compHangar[index_hangar - 1]);
					entity->SetComponent<HangarComponent>(&compHangar[index]);
					index_hangar--;
				}
			}
			index++;
		}

		// NOTE: add player Resource component as well

		// removes entity groups if they are inactive. This is called the erase-remove idiom
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
				[i](Entity *mEntity)
			{
				return !mEntity->IsActive() || !mEntity->HasGroup(i);
			}),
				std::end(v));
		}

		int size = entities.size();
		// removes entities if they are inactive. This is called the erase-remove idiom
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), // iterates through entities
			[](const std::unique_ptr<Entity> &mEntity) // creates unique ptr to each element
		{
			return !mEntity->IsActive(); // for each element, checks if active
		}),
			std::end(entities));

		// NOTE: remove_if (ForwardIterator first, ForwardIterator last, UnaryPredicate pred);
		// NOTE: erase( const_iterator first, const_iterator last );

		// Here, erase(remove_if (it first, it last, UnaryPredicate pred), const_iterator last);
		// "UnaryPredicate pred" becomes [](const std::unique_ptr<Entity> &mEntity

		// this is very efficient, since "remove_if" actually moves all elements that do not fit the remove criterium to the front of the vector. Afterwards, a single call of erase removes all remaining elements at the end of the vector.
	}
}


EntityManager::EntityManager()
{

}

void EntityManager::AddToGroup(Entity * mEntity, Group mGroup)
{
	mEntity->AddToGroup(mGroup);
	groupedEntities[mGroup].emplace_back(mEntity);
}

std::vector<Entity*>& EntityManager::GetGroup(Group mGroup)
{
	return groupedEntities[mGroup];
}

std::vector<Entity*>& EntityManager::FindEntitiesInArea(const Vector2D& position, Group mGroup, const float& dist)
{
	entities_in_area.clear();
	for (auto* e : GetGroup(mGroup))
	{
		if (Collision::CircularCollision(e->GetComponent<TransformComponent>().position, position, dist))
		{
			entities_in_area.push_back(e);
		}
	}
	return entities_in_area;
}

Entity* EntityManager::FindOneEntityInArea(const Vector2D& position, const Group& mGroup, const float& dist)
{
	for (auto* e : GetGroup(mGroup))
	{
		if (Collision::CircularCollision(e->GetComponent<TransformComponent>().position, position, dist))
		{
			return e;
		}
	}
	return nullptr;
}

std::set<Entity*>* EntityManager::getDeletedEntities()
{
	return &deleted_entities;
}