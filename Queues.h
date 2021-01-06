#pragma once
#include "Components.h"
#include <queue>

struct PathfindingRequest
{
	PathfindingRequest() = default;
	PathfindingRequest(Entity* _entity) : entity(_entity) {}
	PathfindingRequest(Entity* _entity, Vector2D _target) : entity(_entity), target(_target) {}
	PathfindingRequest(Entity* _entity, Entity* _targetEntity) : entity(_entity), targetEntity(_targetEntity) {}
	Entity* entity = nullptr;
	Vector2D target;
	Entity* targetEntity = nullptr;
};

class PathfindingQueue
{
public:
	void makePathfindingRequest(Entity * entity);
	void makePathfindingRequest(Entity* entity, const Vector2D& target);
	void makePathfindingRequest(Entity * entity, Entity * targetEntity);
	void executePathfindingRequests(double maxTime);
	void UpdateDeletedEntities(std::set<Entity*> entities);

private:
	std::map<Entity*, PathfindingRequest> pathfinding_requests;
	std::set<Entity*> deletedEntities;
};
