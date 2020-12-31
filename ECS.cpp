#include "ECS.h"

void Entity::Update()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}

void Entity::Draw()
{
	for (auto& comp : components)
	{
		comp->Draw();
	}
}

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

