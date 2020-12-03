#include "NavigationManager.h"
#include <fstream>
#include <map>
#include <queue>
#include "Math.h"
#include "Vector2D.h"

#include <chrono>
using namespace std::chrono;



float heuristic(const Node *n, const Node *target)
{
	return Math::distance(n->x, n->y, target->x, target->y);
}

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

std::stack<Vector2D> NavigationManager::CalculatePath(const Vector2D& curLoc, const Vector2D& targetLoc)
{
	
	//UPGRADES:
	// NEIGHBOURS STORAGE IN NODES


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


	// check if start or target are out of bounds, check if target is obstacle
	std::stack<Vector2D> path;
	if (!current || !target || target->isObstacle)
	{
		return path;
	}



	// list yet to be tested
	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> not_tested;
	not_tested.push(current);


	// visited map
	std::map<int, bool> visited;

	// current parent node map
	std::map<Node*, Node*> parents;

	// fLocal and fGlobal map
	std::map<Node*, int> goals;


	// set up data structures
	for (int y = 0; y < navMesh.cols; y++)
	{
		for (int x = 0; x < navMesh.rows; x++)
		{
			visited[x*y] = false;
			parents[navMesh(x, y)] = nullptr;
			goals[navMesh(x, y)] = INT_MAX;
		}
	}


	// Initialize currrent
	goals[current] = Math::distance(current->x, current->y, target->x, target->y);

	// while there are nodes not yet tested

	int numCalls = 0;
	while (!not_tested.empty() && current != target)
	{
		// sort list by global goal (min). Note that global goal is distance to target
		///not_tested.sort([](const Node* nA, const Node* nB) {return nA->fGlobal < nB->fGlobal; });
		//not_tested.([goals](Node* nA, Node* nB) {return goals.at(nA).second < goals.at(nB).second; });

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
			numCalls++;
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
				float lowestGoal = goals[current] + Math::distance(n->x, n->y, current->x, current->y);

				float local = goals[n];
				// check if the lowest local goal is smaller than the neighbour's previous local goal
				if (lowestGoal < goals[n])
				{
					// set the neighbour's parent to the current node
					parents[n] = current;

					// update the neighbour's local goal
					goals[n] = lowestGoal;

					// set the neighbour's global goal to the local goal plus the distance to the target
					n->globalDist = goals[n] + Math::distance(n->x, n->y, target->x, target->y);
				}

			}

		}


	}
	for (int i = 0; i < navMesh.mesh.size(); i++)
	{
		navMesh.mesh[i]->globalDist = INT_MAX;
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
