#include "Navmesh.h"
#include <fstream>
#include <queue>
#include "Math.h"
#include <chrono>
using namespace std::chrono;

int Node::NodeID = 0;

Navmesh::Navmesh()
{

}

Navmesh::~Navmesh()
{
}

void Navmesh::Init(int mCols, int mRows)
{
	for (int y = 0; y < cols; y++)
	{
		for (int x = 0; x < rows; x++)
		{
			visited[getNodeAt(x, y)->ID] = false;
			parents[getNodeAt(x, y)] = nullptr;
			goals[getNodeAt(x, y)] = INT_MAX;
		}
	}
	cols = mCols;
	rows = mRows;
	neighbours = std::vector<Node*>(8);
}

void Navmesh::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	Init(sX, sY);
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
				mesh.push_back(new Node(x, y, true));
			}
			else
			{
				mesh.push_back(new Node(x, y, false));
			}

			stream.ignore();
		}
	}
	stream.close();
}

std::stack<Vector2D> Navmesh::CalculatePath(Vector2D curLoc, Vector2D targetLoc)
{
	auto start = std::chrono::high_resolution_clock().now();

	int closestX = (curLoc.x / tileSizeX);
	int closestY = (curLoc.y / tileSizeY);

	int closestXTarget = (targetLoc.x / tileSizeX);
	int closestYTarget = (targetLoc.y / tileSizeY);

	// A*
	// Initialize start and end node
	Node* current = getNodeAt(closestX, closestY);
	Node* target = getNodeAt(closestXTarget, closestYTarget);


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
	ClearMesh();

	// Initialize currrent
	goals[current] = Math::distanceNoSqrt(current->x, current->y, target->x, target->y);

	// while there are nodes not yet tested
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
		for (Node * n : getNeighbours(current->x, current->y))
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

	auto end = std::chrono::high_resolution_clock().now();

	auto totaltime = std::chrono::duration_cast<milliseconds>(end - start);
	std::cout << totaltime.count() << std::endl;

	return path;
}

void Navmesh::ClearMesh()
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
	for (int n = 0; n < mesh.size(); ++n)
	{
		mesh[n]->globalDist = INT_MAX;
	}
}

Node * Navmesh::operator()(const int& x, const int& y)
{
	if (boundsCheck(y * cols + x))
	{
		return mesh[y * cols + x];
	}
	else
	{
		return nullptr;
	}
}

Node * Navmesh::getNodeAt(const int& x, const int& y)
{
	int index = y * cols + x;
	if (index >= 0 && index < mesh.size())
	{
		return mesh[index];
	}
	else
	{
		return nullptr;
	}
}

const std::vector<Node*>& Navmesh::getNeighbours(const int& x, const int& y)
{
	int index = y * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[0] = (mesh[index]);
	}
	index = (y + 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[1] = (mesh[index]);
	}
	index = (y + 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		neighbours[2] = (mesh[index]);
	}
	index = (y + 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[3] = (mesh[index]);
	}
	index = (y)* cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[4] = (mesh[index]);
	}
	index = (y - 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[5] = (mesh[index]);
	}
	index = (y - 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		neighbours[6] = (mesh[index]);
	}
	index = (y - 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[7] = (mesh[index]);
	}

	return neighbours;
}

bool Navmesh::boundsCheck(const int& index)
{
	return (index >= 0 && index < mesh.size());
}