#include "ECS.h"

long int Entity::TickMaster = 0;

bool Component::IsActive() const
{
	return active;
}

void Component::Destroy()
{
	active = false;
}

bool Entity::IsActive() const
{
	return active;
}

void Entity::Destroy()
{
	active = false;
	for (auto comp : componentArray)
	{
		if (comp)
		{
			comp->Destroy();
		}
	}
}

Entity::~Entity()
{
	for (auto& comp : componentArray)
	{
		comp = nullptr;
	}
}

bool Entity::HasGroup(Group mGroup)
{
	return groupBitSet[mGroup];
}

void Entity::AddToGroup(Group mGroup)
{
	groupBitSet[mGroup] = true;
}

void Entity::RemoveFromGroup(Group mGroup)
{
	groupBitSet[mGroup] = false;
}