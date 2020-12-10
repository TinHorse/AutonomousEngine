#include "CollisionManager.h"
#include <fstream>

extern EntityManager manager;
int Matrix::matIndex = 1;

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
				auto& tileCol(manager.AddEntity());
				auto& collider = tileCol.AddComponent<ColliderComponent>("terrain", x * tileSizeX, y * tileSizeY, tileSizeX);
				tileCol.AddToGroup(Game::groupColliders);
				colMat.addNode(&collider);
				colMat.mesh.push_back(colMat.matIndex);
			}
			else
			{
				colMat.mesh.push_back(0);
			}

			stream.ignore();
		}
	}
	stream.close();
}

void CollisionManager::AddAgent(const Entity & agent)
{
	dynamic_colliders.push_back(agent.GetComponentPtr<ColliderComponent>());
}

void CollisionManager::CalculateCollision()
{
	//int numCalls = 0;

	for (ColliderComponent* dynCol : dynamic_colliders)
	{
		// determine node position
		int x = dynCol->transform->position.x / tileSizeX;
		int y = dynCol->transform->position.y / tileSizeY;
		
		// check collision with static colliders
		
		for (auto *n : colMat.getRegion(x, y))
		{
			if (n != nullptr)
			{
				//numCalls++;
				if (Collision::AABB(*dynCol, *n))
				{
					//dynCol->entity->GetComponent<TransformComponent>().velocity = { 0,0 };
				}
			}			
		}
		
	}
	//std::cout << "num " << numCalls << std::endl;
}
