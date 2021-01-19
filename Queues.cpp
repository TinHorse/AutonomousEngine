#include "Queues.h"

//extern Navmesh navigation;

void PathfindingQueue::makePathfindingRequest(Entity * entity)
{
	pathfinding_requests[entity] = PathfindingRequest(entity);
}

void PathfindingQueue::makePathfindingRequest(Entity * entity, const Vector2D& target)
{
	pathfinding_requests[entity] = PathfindingRequest(entity, target);
}

void PathfindingQueue::makePathfindingRequest(Entity * entity, Entity * targetEntity)
{
	pathfinding_requests[entity] = PathfindingRequest(entity, targetEntity);
}

void PathfindingQueue::executePathfindingRequests(double maxTime)
{
	double beginTime = SDL_GetTicks();
	double currTime;

	int num_requests = 0;
	while (!pathfinding_requests.empty())
	{
		currTime = SDL_GetTicks();
		if ((currTime - beginTime) >= maxTime) { break; }

		auto mapIndex = pathfinding_requests.begin();
		auto entity = mapIndex->first;
		auto request = mapIndex->second;
		num_requests++;
		// check for deleted entities
		if (deletedEntities.find(entity) == deletedEntities.end())
		{
			if (request.targetEntity)
			{
				if (deletedEntities.find(request.targetEntity) == deletedEntities.end())
				{
					request.entity->GetComponent<PathfindingComponent>().FindPathToTarget(request.targetEntity);
				}
			}
			else if(request.target != Vector2D(0,0))
			{
				request.entity->GetComponent<PathfindingComponent>().FindPath(request.target);
			}
			else
			{
				request.entity->GetComponent<PathfindingComponent>().Explore();
			}
		}

		// clear current request
		pathfinding_requests.erase(entity);
	}
}

void PathfindingQueue::UpdateDeletedEntities(std::set<Entity*>& entities)
{
	deletedEntities.insert(entities.begin(), entities.end());
}
