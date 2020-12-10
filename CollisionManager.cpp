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
	for (Entity* dynCol : manager.GetGroup(Game::groupAgents))
	{

		// determine node position
		int x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		int y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;
		
		// check collision with static colliders
		
		Vector2D force;
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
		dynCol->GetComponent<TransformComponent>().velocity += force;
		dynCol->GetComponent<TransformComponent>().velocity.Normalize() * dynCol->GetComponent<TransformComponent>().speed;
		

		for (Entity* dynCol2 : manager.GetGroup(Game::groupAgents))
		{
			if (dynCol != dynCol2)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), dynCol2->GetComponent<ColliderComponent>()))
				{
					//dynCol->GetComponent<TransformComponent>().velocity * -1;
				}
			}
		}
	}

	for (Entity* dynCol : manager.GetGroup(Game::groupPlayers))
	{

		// determine node position
		int x = dynCol->GetComponent<ColliderComponent>().transform->position.x / tileSizeX;
		int y = dynCol->GetComponent<ColliderComponent>().transform->position.y / tileSizeY;

		// check collision with static colliders

		for (auto *n : colMat.getRegion(x, y))
		{
			if (n != nullptr)
			{
				if (Collision::AABB(dynCol->GetComponent<ColliderComponent>(), *n))
				{
					//dynCol->GetComponent<TransformComponent>().velocity += Collision::CalculateOpposingForce(dynCol->GetComponent<ColliderComponent>(), *n).Normalize();
				}
			}
		}

		Vector2D force;
		for (Entity* dynCol2 : manager.GetGroup(Game::groupColliders))
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
		dynCol->GetComponent<TransformComponent>().velocity.Normalize() * dynCol->GetComponent<TransformComponent>().speed;
	}
}
