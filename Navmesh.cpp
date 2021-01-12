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
				mesh[(y*cols) + x] = Node(x,y,true);
			}
			else
			{
				mesh[(y*cols) + x] = Node(x, y, false);
			}

			stream.ignore();
		}
	}
	stream.close();
}

void Navmesh::CalculatePath(Entity* entity, std::stack<Vector2D>& path, const Vector2D targetLoc, bool earlyExit)
{
	int totalNodes = 0;

	const Vector2D& curLoc = entity->GetComponent<TransformComponent>().position;
	Vector2D targLoc = targetLoc;

	if (Math::distance(curLoc, targetLoc) > 300)
	{
		targLoc = (Vector2D(targLoc - curLoc).Normalize() * 300) + curLoc;
	}

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
		return;

	if (target->isObstacle)
	{
		std::cout << "is Bostacle" << std::endl;
		for (auto& n : getNeighbours(target->x, target->y))
		{
			if (!n->isObstacle)
			{
				std::cout << "no bostacle" << std::endl;
				target = n;
				break;
			}
		}
		if(target->isObstacle)
			return;
	}

	// list yet to be tested
	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> not_tested;
	not_tested.push(current);

	// set up data structures
	ClearMesh();

	// Initialize currrent
	goals[current] = Math::distanceNoSqrt(current->x, current->y, target->x, target->y);

	int num_nodes_tested = 0;
	int max_nodes_to_test = Math::distanceNoSqrt(current->x, current->y, target->x, target->y);
	//std::cout << "max nodse to test " << max_nodes_to_test << " at distance " << sqrt(goals[current]) << std::endl;
	// while there are nodes not yet tested
	while (!not_tested.empty() && current != target)
	{
		if (num_nodes_tested++ > max_nodes_to_test)
			std::cout << "LIMIT REACHED : " << num_nodes_tested << std::endl;

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
				long int lowestGoal = goals[current] + Math::distanceNoSqrt(n->x, n->y, current->x, current->y);

				long int local = goals[n];
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

	if (!path.empty())
		path.pop();
		//if (!path.empty())
			//path.pop();
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

const std::array<Node*, 8>& Navmesh::getNeighbours(const int& x, const int& y)
{
	int index = y * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[0] = (&mesh[index]);
	}
	index = (y + 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[1] = (&mesh[index]);
	}
	index = (y + 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		neighbours[2] = (&mesh[index]);
	}
	index = (y + 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[3] = (&mesh[index]);
	}
	index = (y)* cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[4] = (&mesh[index]);
	}
	index = (y - 1) * cols + x - 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[5] = (&mesh[index]);
	}
	index = (y - 1) * cols + x;
	if (index >= 0 && index < mesh.size()) {
		neighbours[6] = (&mesh[index]);
	}
	index = (y - 1) * cols + x + 1;
	if (index >= 0 && index < mesh.size()) {
		neighbours[7] = (&mesh[index]);
	}

	return neighbours;
}

bool Navmesh::boundsCheck(const int& index)
{
	return (index >= 0 && index < mesh.size());
}