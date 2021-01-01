#include "ECS.h"

bool Entity::IsActive() const
{
	return active;
}

void Entity::Destroy()
{
	active = false;
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

