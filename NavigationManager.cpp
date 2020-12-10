#include "NavigationManager.h"
#include <fstream>

#include <queue>
#include "Math.h"

NavigationManager::NavigationManager()
{
	
}

NavigationManager::~NavigationManager()
{
}

void NavigationManager::Init()
{
	for (int y = 0; y < navMesh.cols; y++)
	{
		for (int x = 0; x < navMesh.rows; x++)
		{
			visited[navMesh(x, y)->ID] = false;
			parents[navMesh(x, y)] = nullptr;
			goals[navMesh(x, y)] = INT_MAX;
		}
	}
}

void NavigationManager::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	navMesh.Init(sX, sY);
	tileSizeX = sTileX * scale;
	tileSizeY = sTileY * scale;

	char c;
	std::fstream stream;
	stream.open(path);
	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sX; ++x)
		{
			stream.get(c);
			if (c == '1')
			{
				navMesh.mesh.push_back(new Node(x,y, true));
			}
			else
			{
				navMesh.mesh.push_back(new Node(x, y, false));
			}

			stream.ignore();
		}
	}
	stream.close();
}

std::stack<Vector2D> NavigationManager::CalculatePath(const Vector2D& curLoc, const Vector2D& targetLoc)
{
	int closestX = (curLoc.x / tileSizeX);
	int closestY = (curLoc.y / tileSizeY);

	int closestXTarget = (targetLoc.x / tileSizeX);
	int closestYTarget = (targetLoc.y / tileSizeY);

	// A*
	// Initialize start and end node
	Node* current = navMesh(closestX, closestY);
	Node* target = navMesh(closestXTarget, closestYTarget);


	// check if start or target are out of bounds, check if target is obstacle
	std::stack<Vector2D> path;
	if (!current || !target || target->isObstacle)
	{
		return path;
	}

	// list yet to be tested
	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> not_tested;
	not_tested.push(current);

	// set up data structures
	Restart();

	// Initialize currrent
	goals[current] = Math::distanceNoSqrt(current->x, current->y, target->x, target->y);

	// while there are nodes not yet tested

	int numCalls = 0;
	while (!not_tested.empty() && current != target)
	{
		// if the node has been visited, remove it
		while (!not_tested.empty() && visited[not_tested.top()->ID])
		{
			not_tested.pop();
		}

		// if the list is empty now, terminate
		if (not_tested.empty())
		{
			break;
		}

		// set current node to front of the list
		current = not_tested.top();
		visited[current->ID] = true;


		// check all neighbours
		for (Node * n : navMesh.getNeighbours(current->x, current->y))
		{
			if (n != nullptr)
			{
				// check if neighbour has already been visited and make sure it is not an obstacle
				bool vis = visited[n->ID];
				bool isObstacle = n->isObstacle;
				if (!visited[n->ID] && !n->isObstacle)
				{
					not_tested.push(n);
				}

				// calculate the neighbour's local goals
				float lowestGoal = goals[current] + Math::distanceNoSqrt(n->x, n->y, current->x, current->y);

				float local = goals[n];
				// check if the lowest local goal is smaller than the neighbour's previous local goal
				if (lowestGoal < goals[n])
				{
					// set the neighbour's parent to the current node
					parents[n] = current;

					// update the neighbour's local goal
					goals[n] = lowestGoal;

					// set the neighbour's global goal to the local goal plus the distance to the target
					n->globalDist = goals[n] + Math::distanceNoSqrt(n->x, n->y, target->x, target->y);
				}
			}
		}
	}

	// Backtrack through the parent map to find the final path
	if (current != nullptr)
	{
		while (target != nullptr)
		{
			path.push(Vector2D(target->x * tileSizeX, target->y * tileSizeY));
			target = parents[target];
		}
	}
	path.pop();
	path.push(curLoc);

	return path;
}

void NavigationManager::Restart()
{
	for (auto it = visited.begin(); it != visited.end(); ++it)
	{
		it->second = false;
	}
	for (auto it = parents.begin(); it != parents.end(); ++it)
	{
		it->second = nullptr;
	}
	for (auto it = goals.begin(); it != goals.end(); ++it)
	{
		it->second = INT_MAX;
	}
	for (int n = 0; n < navMesh.mesh.size(); ++n)
	{
		navMesh.mesh[n]->globalDist = INT_MAX;
	}
}
