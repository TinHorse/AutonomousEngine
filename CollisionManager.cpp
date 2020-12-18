#include "CollisionManager.h"
#include "Collision.h"
#include <fstream>

extern EntityManager manager;
int CollisionMesh::matIndex = 1;

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

void CollisionManager::CalculateCollision()
{
	int colcount = 0;
	Vector2D force;
	int x, y;
	bool staticCol = false;
	for (Entity* dynCol : manager.GetGroup(Game::groupAgents))
	{
		// determine node position
		x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;

		// check collision with static colliders
		force.Zero();
		staticCol = false;

		for (auto *n : colMat.getRegion(x, y))
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
		staticCol = false;

		// determine node position
		x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;

		// check collision with static colliders
		force.Zero();
		for (auto *n : colMat.getRegion(x, y))
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
		dynCol->GetComponent<ColliderComponent>().transform->velocity += force;
		dynCol->GetComponent<ColliderComponent>().transform->velocity.Normalize();
	}
}
