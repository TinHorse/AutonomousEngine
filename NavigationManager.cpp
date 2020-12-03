#include "NavigationManager.h"
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include "Math.h"
#include "Vector2D.h"
#include <array>
#include <vector>


NavigationManager::NavigationManager()
{
	
}

NavigationManager::~NavigationManager()
{
}

void NavigationManager::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	navMesh.Init(sX, sY);
	//matrix. = std::vector<int>(sX*sY);
	//navMesh.mat = std::vector<Node*>(sX*sY, new Node(false, false, INT_MAX, INT_MAX,x,y));
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
				///navMesh(x, y) = atoi(&c);
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
	Node* target = navMesh(closestXTarget, closestYTarget);

	std::stack<Vector2D> path;
	if (!current || !target)
	{
		return path;
	}


	// list yet to be tested
	std::list<Node*> not_tested;
	not_tested.push_back(current);
	

	// visited map
	std::map<Node*, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;
	
	// fLocal and fGlobal map
	std::map<Node*, std::pair<int, int>> goals;

	// set up data structures
	for (int y = 0; y < navMesh.cols; y++)
	{
		for (int x = 0; x < navMesh.rows; x++)
		{
			visited[navMesh(x, y)] = false;
			parents[navMesh(x, y)] = nullptr;
			goals[navMesh(x, y)] = std::make_pair(10000,10000);
		}
	}

	// Initialize currrent
	goals[current] = std::make_pair(0.f, Math::distance(current->x, current->y, target->x, target->y));


	// neighbour nodes
	std::vector<Node*> neighbours;

	// while there are nodes not yet tested
	while (!not_tested.empty())
	{
		// sort list by global goal (min). Note that global goal is distance to target
		///not_tested.sort([](const Node* nA, const Node* nB) {return nA->fGlobal < nB->fGlobal; });
		not_tested.sort([goals](Node* nA, Node* nB) {return goals.at(nA).second < goals.at(nB).second; });

		// if the node has been visited, remove it
		while (!not_tested.empty() && visited[not_tested.front()])
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
		visited[current] = true;

		// populate neighbours vector
		neighbours.clear();
		neighbours.push_back(navMesh(current->x+1, current->y));
		neighbours.push_back(navMesh(current->x+1, current->y+1));
		neighbours.push_back(navMesh(current->x, current->y+1));
		neighbours.push_back(navMesh(current->x-1, current->y+1));
		neighbours.push_back(navMesh(current->x-1, current->y));
		neighbours.push_back(navMesh(current->x-1, current->y-1));
		neighbours.push_back(navMesh(current->x, current->y-1));
		neighbours.push_back(navMesh(current->x + 1, current->y - 1));
		
		// check all neighbours
		for (Node * n : neighbours)
		{
			if (n != nullptr)
			{
				// check if neighbour has already been visited and make sure it is not an obstacle
				bool vis = visited[n];
				bool isObstacle = n->isObstacle;
				if (!visited[n] && !n->isObstacle)
				{
					not_tested.push_back(n);
				}

				// calculate the neighbour's local goals
				float lowestGoal = goals[current].first + Math::distance(n->x, n->y, current->x, current->y);

				float local = goals[n].first;
				// check if the lowest local goal is smaller than the neighbour's previous local goal
				if (lowestGoal < goals[n].first)
				{
					// set the neighbour's parent to the current node
					parents[n] = current;

					// update the neighbour's local goal
					goals[n].first = lowestGoal;

					// set the neighbour's global goal to the local goal plus the distance to the target
					goals[n].second = goals[n].first +Math::distance(n->x, n->y, target->x, target->y);
				}

			}

		}



	}

	//std::stack<Vector2D> path;
	if (current != nullptr)
	{
		while (target != nullptr)
		{
			path.push(Vector2D(target->x * tileSizeX, target->y * tileSizeY));
			target = parents[target];
		}
	}
	path.push(curLoc);

	return path;
}
