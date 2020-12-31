#pragma once
#include "ECS.h"
#include "Components.h"

class EntityManager
{
public:
	void Update();
	void Draw();
	void Refresh();

	void AddToGroup(Entity *entity, Group mGroup); // adds an entity to one of the groups
	Entity& AddEntity();
	std::vector<Entity*>& GetGroup(Group mGroup); // returns the specified group of entities

	

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; // same as entities, but grouped for rendering

	std::vector<Component> colliderComponents;
};

