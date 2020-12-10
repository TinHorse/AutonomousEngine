#include "CollisionManager.h"
#include <fstream>

extern EntityManager manager;

void CollisionManager::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	colMat.Init(sX, sY);
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
				colMat.mesh.push_back(new CollisionNode(x, y, true));
				auto& tileCol(manager.AddEntity());
				tileCol.AddComponent<ColliderComponent>("terrain", x * tileSizeX, y * tileSizeY, tileSizeX);
				tileCol.AddToGroup(Game::groupColliders);
			}
			else
			{
				colMat.mesh.push_back(new CollisionNode(x, y, false));
			}

			stream.ignore();
		}
	}
	stream.close();
}

void CollisionManager::AddAgent(const Entity & agent)
{
	Vector2D v = agent.GetComponent<TransformComponent>().position;

	colMat(v.x / tileSizeX, v.y / tileSizeY)->colliders.insert(agent.GetComponentPtr<ColliderComponent>());
	dynamic_colliders.push_back(agent.GetComponentPtr<ColliderComponent>());
}

void CollisionManager::CalculateCollision()
{
	int numCalls = 0;
	CollisionNode *colNode;

	for (ColliderComponent* dynCol : dynamic_colliders)
	{
		// determine node position
		int x = dynCol->transform->position.x / tileSizeX;
		int y = dynCol->transform->position.y / tileSizeY;

		colNode = colMat(x, y);

		// if not registered on tile, register collider
		
		if (colNode->colliders.find(dynCol) == colNode->colliders.end())
		{
			colNode->colliders.insert(dynCol);
		}
		
		

		// check collision with other dynamic colliders
		
		for (auto* col : colNode->colliders)
		{
			if (col != dynCol)
			{
				Collision::AABB(col->collider, dynCol->collider);
			}
			numCalls++;
		}
		
		// check collision with static colliders
		
		for (auto *n : colMat.getRegion(x, y))
		{
			
			if (n->isObstacle)
			{
				numCalls++;
				Collision::AABB(dynCol->collider, n->x, n->y, tileSizeX, tileSizeY);
			}
			
			for (auto *c : n->colliders)
			{
				numCalls++;
				Collision::AABB(c->collider, dynCol->collider);
				//std::cout << dynCol->entity << " colliding with " << c->entity << std::endl;
			}
			
		}
		
	}
	std::cout << "num " << numCalls << std::endl;
}
