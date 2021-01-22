#include "Navmesh.h"
#include <fstream>
#include <queue>
#include "Math.h"
#include "Components.h"

int Node::NodeID = 0;

Navmesh::Navmesh()
{

}

Navmesh::~Navmesh()
{

}

void Navmesh::Init(int mCols, int mRows)
{
	mesh = std::vector<Node>(mCols * mRows);
	cols = mCols;
	rows = mRows;

	for (int y = 0; y < cols; y++)
	{
		for (int x = 0; x < rows; x++)
		{
			visited[getNodeAt(x, y)->ID] = false;
			parents[getNodeAt(x, y)] = nullptr;
			goals[getNodeAt(x, y)] = LONG_MAX;
		}
	}

	ClearMesh();
}

void Navmesh::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	int scalar = 2;
	Init(sX*scalar, sY*scalar);
	tileSizeX = sTileX * scale;
	tileSizeY = sTileY * scale;

	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sX; ++x)
		{
			mesh[(y*cols) + x] = Node(x,y,true);
		}
	}

	char c;
	std::fstream stream;
	stream.open(path);
	for (int y = 0; y < sY; y += scalar)
	{
		for (int x = 0; x < sX; x += scalar)
		{
			stream.get(c);
			if (c == '1')
			{
				//mesh[(y*cols) + x] = Node(x, y, true);
			}
			else
			{
				//mesh[(y*cols) + x] = Node(x, y, false);
			}

			stream.ignore();
		}
	}
	stream.close();
}

bool Navmesh::CalculatePath(Entity* entity, std::stack<Vector2D>& path, const Vector2D targetLoc)
{
	bool path_change = false;

	const Vector2D& curLoc = entity->GetComponent<TransformComponent>().position;
	Vector2D targLoc = targetLoc;

	int closestX = (curLoc.x / tileSizeX);
	int closestY = (curLoc.y / tileSizeY);

	int closestXTarget = (targLoc.x / tileSizeX);
	int closestYTarget = (targLoc.y / tileSizeY);

	// A*
	// Initialize start and end node
	Node* current = getNodeAt(closestX, closestY);
	Node* target = getNodeAt(closestXTarget, closestYTarget);
	
	// check if start or target are out of bounds, check if target is obstacle
	if (!current || !target)
		return false;

	// check if distance is greater than the limit
	if (Math::distance(current->x, current->y, target->x, target->y) > 9)
	{
		//std::cout << Math::distance(curLoc, targLoc) << " dist" << std::endl;
		int targX = target->x;
		int targY = target->y;
		target = getNodeAt((targX - current->x) / 2 + current->x, (targY - current->y) / 2 + current->y);
		// set up data structures
		path_change = true;
		ClearMesh();
	}

	// check if the distance between this and the target node is smaller than 2. If it is, return. Else, clear data structures
	else if (Math::distance(current->x, current->y, target->x, target->y) < 2)
	{
		path.push(Vector2D(target->x * tileSizeX, target->y * tileSizeY));
		return true;
	}
	else
	{
		// set up data structures
		ClearMesh();
	}


	//check if the target is an obstacle. If it is, search the nearby nodes for a non-obstacle
	if (target->isObstacle)
	{
		for (auto* n : getNeighbours(target->x, target->y))
		{
			target = n;
			path_change = true;
			break;
		}
		if(target->isObstacle)
			return false;
	}

	// If the target is still the original target, push it
	if(!path_change)
		path.push(targetLoc);

	// list yet to be tested
	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> not_tested;
	not_tested.push(current);

	// Initialize currrent
	goals[current] = Math::distanceNoSqrt(current->x, current->y, target->x, target->y);

	//int num_nodes_tested = 0;
	//int max_nodes_to_test = 50;
	//std::cout << "max nodse to test " << max_nodes_to_test << " at distance " << sqrt(goals[current]) << std::endl;
	// while there are nodes not yet tested
	while (!not_tested.empty() && current != target)
	{
		//if (num_nodes_tested++ > max_nodes_to_test)
			//std::cout << "LIMIT REACHED : " << num_nodes_tested << std::endl;

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

		bool vis;
		long int lowestGoal;
		long int local;

		// check all neighbours
		for (auto* n : getNeighbours(current->x, current->y))
		{
			// check if neighbour has already been visited and make sure it is not an obstacle
			vis = visited[n->ID];
			if (!visited[n->ID])
			{
				not_tested.push(n);
			}

			// calculate the neighbour's local goals
			lowestGoal = goals[current] + Math::distanceNoSqrt(n->x, n->y, current->x, current->y);

			local = goals[n];
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

	// Backtrack through the parent map to find the final path
	if (current != nullptr)
	{
		while (target != nullptr)
		{
			path.push(Vector2D(target->x * tileSizeX, target->y * tileSizeY));
			target = parents[target];
		}
	}

	if (!path.empty())
		path.pop();
	if (!path.empty())
		return true;
	else
		return false;
}

void Navmesh::ClearMesh()
{
	for (auto& n : mesh)
	{
		n.globalDist = LONG_MAX;
	}

	for (auto& it : visited)
	{
		it.second = false;
	}
	for (auto &it : parents)
	{
		it.second = nullptr;
	}
	for (auto& it : goals)
	{
		it.second = LONG_MAX;
	}
}

Node & Navmesh::operator()(const int& x, const int& y)
{
	if (boundsCheck(y * cols + x))
	{
		return mesh[y * cols + x];
	}
	else
	{
		return nullNode;
	}
}

Node * Navmesh::getNodeAt(const int& x, const int& y)
{
	int index = y * cols + x;
	if (index >= 0 && index < mesh.size())
	{
		return &mesh[index];
	}
	else
	{
		return nullptr;
	}
}

const std::vector<Node*>& Navmesh::getNeighbours(const int& x, const int& y)
{
	neighbours.clear();
	int index = y * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		if(!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y + 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y + 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y + 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y)* cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y - 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y - 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}
	index = (y - 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		if (!mesh[index].isObstacle)
			neighbours.push_back(&mesh[index]);
	}

	return neighbours;
}

bool Navmesh::boundsCheck(const int& index)
{
	return (index >= 0 && index < mesh.size());
}