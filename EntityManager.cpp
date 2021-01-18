#include "EntityManager.h"
#include "Collision.h"

void EntityManager::Update()
{
	int index = 0;
	for (auto& comp : compTran)
	{
		if (index++ >= index_tran) { break; };
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
	index = index_static_coll;
	for (auto& comp : compStaticColl)
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
	index = index_tiles;
	for (auto& comp : compTile)
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
	index = index_state;
	for (auto& comp : compState)
	{
		if (index-- <= 0) { break; };
		comp.Update();
		comp.UpdateTargetEntities(deleted_entities);
	}
}

void EntityManager::Refresh()
{
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
		for (auto& e : GetGroup(Game::groupHunted))
		{
			e->refresh(deleted_entities);
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
		for (auto& comp : compState)
		{
			if (index >= index_state) { break; };
			if (!comp.IsActive())
			{
				while (index_state > 0 && !compState[index_state - 1].IsActive())
				{
					index_state--;
				}
				if (index_state > 0 && index_state != index)
				{
					Entity* entity = compState[index_state - 1].entity;
					std::swap(compState[index], compState[index_state - 1]);
					entity->SetComponent<StateComponent>(&compState[index]);
					index_state--;
				}
			}
			index++;
		}

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
	for (auto& e : GetGroup(mGroup))
	{
		if (Collision::CircularCollision(e->GetComponent<TransformComponent>().position, position, dist))
		{
			entities_in_area.push_back(e);
		}
	}
	return entities_in_area;
}

Entity* EntityManager::FindOneEntityInArea(const Vector2D& position, Group mGroup, const float& dist)
{
	for (auto& e : GetGroup(mGroup))
	{
		if (Collision::CircularCollision(e->GetComponent<TransformComponent>().position, position, dist))
		{
			return e;
		}
	}
	return nullptr;
}

std::set<Entity*>& EntityManager::getDeletedEntities()
{
	return deleted_entities;
}