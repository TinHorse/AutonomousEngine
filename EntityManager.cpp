#include "EntityManager.h"

void EntityManager::Update()
{
	for (int i = 0; i < index_tran; i++)
	{
		compTran[i].Update();
	}
	for (int i = 0; i < index_sprites; i++)
	{
		compSprite[i].Update();
	}
	for (int i = 0; i < index_keyboard; i++)
	{
		compKeyboard[i].Update();
	}
	for (int i = 0; i < index_static_coll; i++)
	{
		compStaticColl[i].Update();
	}
	for (int i = 0; i < index_dynamic_coll; i++)
	{
		compDynamicColl[i].Update();
	}
	for (int i = 0; i < index_tiles; i++)
	{
		compTile[i].Update();
	}
	for (int i = 0; i < index_path; i++)
	{
		compPath[i].Update();
	}
	for (int i = 0; i < index_ai; i++)
	{
		compAI[i].Update();
	}
}

void EntityManager::Refresh()
{
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
