#include "EntityManager.h"

void EntityManager::Update()
{
	int index = index_tran;
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
	}
}

void EntityManager::Refresh()
{
	// remove inactive components
	int index = 0;
	for (auto& comp : compTran)
	{
		if (index++ <= index_tran) { break; };
		if (!comp.IsActive())
		{
			while (index_tran > 0 && !compTran[index_tran - 1].IsActive())
			{
				index_tran--;
			}
			if (index_tran > 0)
			{
				Entity* entity = compTran[index_tran - 1].entity;
				std::cout << compTran[index].position << " swapped with " << compTran[index_tran - 1].position;
				std::swap(compTran[index], compTran[index_tran - 1]);
				entity->SetComponent<TransformComponent>(&compTran[index]);
				index_tran--;
			}
		}
	}

	index = index_sprites;
	for (auto& comp : compSprite)
	{
		if (index++ <= index_sprites) { break; };
		if (!comp.IsActive())
		{
			while (index_sprites > 0 && !compSprite[index_sprites - 1].IsActive())
			{
				index_sprites--;
			}
			if (index_sprites > 0)
			{
				Entity* entity = compSprite[index_sprites - 1].entity;
				std::swap(compSprite[index], compSprite[index_sprites - 1]);
				entity->SetComponent<SpriteComponent>(&compSprite[index]);
				index_sprites--;
			}
		}
	}

	index = index_dynamic_coll;
	for (auto& comp : compDynamicColl)
	{
		if (index++ <= index_dynamic_coll) { break; };
		if (!comp.IsActive())
		{
			while (index_dynamic_coll > 0 && !compDynamicColl[index_dynamic_coll - 1].IsActive())
			{
				index_dynamic_coll--;
			}
			if (index_dynamic_coll > 0)
			{
				Entity* entity = compDynamicColl[index_dynamic_coll - 1].entity;
				std::swap(compDynamicColl[index], compDynamicColl[index_dynamic_coll - 1]);
				entity->SetComponent<ColliderComponent>(&compDynamicColl[index]);
				index_dynamic_coll--;
			}
		}
	}

	index = index_state;
	for (auto& comp : compState)
	{
		if (index++ <= index_state) { break; };
		if (!comp.IsActive())
		{
			while (index_state > 0 && !compState[index_state - 1].IsActive())
			{
				index_state--;
			}
			if (index_state > 0)
			{
				Entity* entity = compState[index_state - 1].entity;
				std::swap(compState[index], compState[index_state - 1]);
				entity->SetComponent<StateComponent>(&compState[index]);
				index_state--;
			}
		}
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


EntityManager::EntityManager()
{

}

void EntityManager::AddToGroup(Entity * mEntity, Group mGroup)
{
	mEntity->AddToGroup(mGroup);
	groupedEntities[mGroup].emplace_back(mEntity);
}

Entity & EntityManager::AddEntity()
{
	Entity *entity = new Entity(*this);

	std::unique_ptr<Entity> uPtr{ entity }; // creates a new unique_ptr that points to the entity. Note that assignment from standard pointer to unique pointer cannot be done through uPtr = entity but must instead be done via {} operator
	entities.emplace_back(std::move(uPtr)); // "emplace_back" gets rid of having to do an unnecessary copy of the element, instead just using the data that is already there at the same memory location. Using "std::move" in conjunction with "emplace_back" is only useful if the element already exists, which is true in this case
	return *entity;
}

std::vector<Entity*>& EntityManager::GetGroup(Group mGroup)
{
	return groupedEntities[mGroup];
}
