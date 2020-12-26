#include "CollisionMesh.h"
#include "Collision.h"
#include <fstream>

int Collisionmesh::matIndex = 1;

int Collisionmesh::operator()(const int& x, const int& y)
{
	if (boundsCheck(y * cols + x))
	{
		return mesh[y * cols + x];
	}
	else
	{
		return 0;
	}
}

const std::vector<ColliderComponent*>& Collisionmesh::getRegion(const int& x, const int& y)
{
	neighbours[0] = getNodeAtPosition(y* cols + x);
	neighbours[1] = getNodeAtPosition(y* cols + x + 1);
	neighbours[2] = getNodeAtPosition((y + 1) * cols + x + 1);
	neighbours[3] = getNodeAtPosition((y + 1) * cols + x);
	neighbours[4] = getNodeAtPosition((y + 1) * cols + x - 1);
	neighbours[5] = getNodeAtPosition(y* cols + x - 1);
	neighbours[6] = getNodeAtPosition((y - 1) * cols + x - 1);
	neighbours[7] = getNodeAtPosition((y - 1) * cols + x);
	neighbours[8] = getNodeAtPosition((y - 1) * cols + x + 1);

	return neighbours;
}

bool Collisionmesh::boundsCheck(const int& index)
{
	return (index >= 0 && index < mesh.size());
}

bool Collisionmesh::doesNodeExist(const int& index)
{
	return (index >= 0 && index < mesh.size() && mesh[index] != 0);
}

ColliderComponent* Collisionmesh::getNodeAtPosition(const int& index)
{
	if (index >= 0 && index < mesh.size() && mesh[index] != 0)
	{
		return nodes[(mesh[index])];
	}
	return nullptr;
}

void Collisionmesh::addNode(ColliderComponent* comp)
{
	nodes.insert(std::make_pair(++matIndex, comp));
}

void Collisionmesh::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	cols = sX;
	rows = sY;
	neighbours = std::vector<ColliderComponent*>(9);
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
				auto& tileCol(manager.AddEntity());
				auto& collider = tileCol.AddComponent<ColliderComponent>("terrain", x * tileSizeX, y * tileSizeY, tileSizeX);
				tileCol.AddToGroup(Game::groupColliders);
				addNode(&collider);
				mesh.push_back(matIndex);
			}
			else
			{
				mesh.push_back(0);
			}

			stream.ignore();
		}
	}
	stream.close();
}

void Collisionmesh::CalculateCollision()
{
	int colcount = 0;
	Vector2D force;
	int x, y;
	for (Entity* dynCol : manager.GetGroup(Game::groupAgents))
	{
		// determine node position
		x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;

		// check collision with static colliders
		force.Zero();
		bool staticCol = false;

		for (auto *n : getRegion(x, y))
		{
			if (n != nullptr)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), *n))
				{
					force += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), *n);
					staticCol = true;
				}
			}
		}

		// check collision with other agents
		if (!staticCol)
		{
			for (Entity* dynCol2 : manager.GetGroup(Game::groupAgents))
			{
				if (dynCol != dynCol2)
				{
					if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>()))
					{
						force += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>());
					}
				}
			}
		}

		for (Entity* dynCol2 : manager.GetGroup(Game::groupPlayers))
		{
			if (dynCol != dynCol2)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>()))
				{
					force += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>());
				}
			}
		}

		dynCol->GetComponent<TransformComponent>().velocity += force;
		dynCol->GetComponent<TransformComponent>().velocity.Normalize();
	}



	for (Entity* dynCol : manager.GetGroup(Game::groupPlayers))
	{
		bool colliding = false;
		// determine node position
		x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;

		// check collision with static colliders
		force.Zero();
		for (auto *n : getRegion(x, y))
		{
			if (n != nullptr)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), *n))
				{
					force += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), *n);
				}
			}
		}

		// check collision with other agents
		for (Entity* dynCol2 : manager.GetGroup(Game::groupAgents))
		{
			if (dynCol != dynCol2)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>()))
				{
					force += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>());
				}
			}
		}
		dynCol->GetComponent<ColliderComponent>().transform->velocity += force;
		dynCol->GetComponent<ColliderComponent>().transform->velocity.Normalize();
	}
}
