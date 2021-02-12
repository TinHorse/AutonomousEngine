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
	double beginTime = SDL_GetTicks() - 3.f;
	double currTime;

	int num_requests = 0;
	while (!pathfinding_requests.empty())
	{
		currTime = SDL_GetTicks();
		///std::cout << " diff " << ((currTime - beginTime)) <<std::endl;
		if ((currTime - beginTime) >= maxTime) { break; }

		auto mapIndex = pathfinding_requests.begin();
		auto entity = mapIndex->first;
		auto request = mapIndex->second;
		num_requests++;
		// check for deleted entities
		if (request.targetEntity)
		{
			request.entity->GetComponent<PathfindingComponent>().FindPathToTarget(request.targetEntity);
		}
		else if (request.target != Vector2D(0, 0))
		{
			request.entity->GetComponent<PathfindingComponent>().FindPath(request.target);
		}
		else
		{
			request.entity->GetComponent<PathfindingComponent>().Explore();
		}

		// clear current request
		pathfinding_requests.erase(entity);
	}
}

void PathfindingQueue::refresh()
{
	for (auto e : *deletedEntities)
	{
		if (pathfinding_requests.find(e) != pathfinding_requests.end())
		{
			pathfinding_requests.erase(e);
		}
		for (auto entity : pathfinding_requests)
		{
			if (entity.second.targetEntity == e)
			{
				pathfinding_requests.erase(e);
			}
		}
	}
}
