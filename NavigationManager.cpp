#include "NavigationManager.h"
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include "Math.h"

NavigationManager::NavigationManager()
{
	
}

NavigationManager::~NavigationManager()
{
}

void NavigationManager::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	navMesh.Init(sTileX, sTileY);
	//matrix. = std::vector<int>(sX*sY);
	//navMesh.mat = std::vector<Node*>(sX*sY, new Node(false, false, INT_MAX, INT_MAX,x,y));
	tileSizeX = sTileX * scale;
	tileSizeY = sTileY * scale;

	char c;
	std::fstream stream;
	stream.open(path);
	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sY; ++x)
		{
			stream.get(c);
			if (c == '1')
			{
				//navMesh(x, y) = atoi(&c);
				navMesh(x, y)->x = x;
				navMesh(x, y)->y = y;
			}
			stream.ignore();
		}
	}
	stream.close();
}

std::stack<Vector2D> NavigationManager::CalculatePath(Vector2D curLoc, Vector2D targetLoc)
{
	//UPGRADES
	// UPGRADE list to priority queue
	// UPGRADE NODE* TO Node ID
	
	//std::queue<Vector2D> path;
	//path.push(curLoc);
	int closestX = (curLoc.x / tileSizeX);
	int closestY = (curLoc.y / tileSizeY);

	//path.push(Vector2D(closestX * tileSizeX, closestY * tileSizeY));

	int closestXTarget = (targetLoc.x / tileSizeX);
	int closestYTarget = (targetLoc.y / tileSizeY);

	// A*
	// Initialize start and end node
	Node* current = navMesh(closestX, closestY);
	Node* goal = navMesh(closestXTarget, closestYTarget);

	// visited map, fLocal and fGlobal, list for nodes yet to be visited, current parent Node map

	// list yet to be tested
	std::list<Node*> not_tested;
	not_tested.push_back(current);

	// visited map
	std::map<Node*, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;

	
	// fLocal and fGlobal map
	std::map<Node*, std::pair<int, int>> costs;


	std::vector<Node*> neighbours(8);

	// while there are nodes not yet tested
	while (!not_tested.empty())
	{
		// sort list by global goal (min). Note that global goal is distance to target
		not_tested.sort([](const Node* nA, const Node* nB) {return nA->fGlobal < nB->fGlobal; });

		// if the node has been visited, remove it
		while (!not_tested.empty() && not_tested.front()->visited)
		{
			not_tested.pop_front();
		}

		// if the list is empty now, terminate
		if (not_tested.empty())
		{
			break;
		}

		// set current node to front of the list
		current = not_tested.front();
		current->visited = true;

		// populate neighbours vector
		neighbours[0] = navMesh(current->x+1, current->y);
		neighbours[1] = navMesh(current->x+1, current->y+1);
		neighbours[2] = navMesh(current->x, current->y+1);
		neighbours[3] = navMesh(current->x-1, current->y+1);
		neighbours[4] = navMesh(current->x-1, current->y);
		neighbours[5] = navMesh(current->x-1, current->y-1);
		neighbours[6] = navMesh(current->x, current->y-1);
		neighbours[7] = navMesh(current->x+1, current->y-1);

		// check all neighbours
		for (Node * n : neighbours)
		{
			if (n != nullptr)
			{
				// check if neighbour has already been visited and make sure it is not an obstacle
				if (!n->visited && !n->isObstacle)
				{
					not_tested.push_back(n);
				}

				// calculate the neighbour's local goals
				Vector2D vN(n->x, n->y);
				Vector2D vCurr(current->x, current->y);
				float lowestGoal = n->fLocal + Math::distance(vN, vCurr);

				// check if the lowest local goal is smaller than the neighbour's previous local goal
				if (lowestGoal < n->fLocal)
				{
					// set the neighbour's parent to the current node
					n->parent = current;
					// update the neighbour's local goal
					n->fLocal = lowestGoal;

					// set the neighbour's global goal to the local goal plus the distance to the target
					n->fGlobal = n->fLocal + Math::distance(vN, targetLoc);
				}
			}

		}



	}

	std::stack<Vector2D> finalPath;
	if (current != nullptr)
	{
		while (goal->parent != nullptr)
		{
			finalPath.push(Vector2D(goal->x, goal->y));
		}
	}


	//path.push(targetLoc);
	return finalPath;
	
	std::stack<Vector2D> s;
	return s;
}
