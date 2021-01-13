#include "CollisionMesh.h"
#include "Collision.h"
#include <fstream>
#include "EntityManager.h"
extern EntityManager manager;

int Collisionmesh::mesh_index = 1;

int Collisionmesh::operator()(const int& x, const int& y)
{
	if (boundsCheck(y * cols + x))
	{
		return collision_mesh[y * cols + x];
	}
	else
	{
		return 0;
	}
}

const std::array<ColliderComponent*,9>& Collisionmesh::getRegion(const int& x, const int& y)
{
	mesh_neighbours[0] = getNodeAtPosition(y* cols + x);
	mesh_neighbours[1] = getNodeAtPosition(y* cols + x + 1);
	mesh_neighbours[2] = getNodeAtPosition((y + 1) * cols + x + 1);
	mesh_neighbours[3] = getNodeAtPosition((y + 1) * cols + x);
	mesh_neighbours[4] = getNodeAtPosition((y + 1) * cols + x - 1);
	mesh_neighbours[5] = getNodeAtPosition(y* cols + x - 1);
	mesh_neighbours[6] = getNodeAtPosition((y - 1) * cols + x - 1);
	mesh_neighbours[7] = getNodeAtPosition((y - 1) * cols + x);
	mesh_neighbours[8] = getNodeAtPosition((y - 1) * cols + x + 1);

	return mesh_neighbours;
}

bool Collisionmesh::boundsCheck(const int& index)
{
	return (index >= 0 && index < collision_mesh.size());
}

bool Collisionmesh::doesNodeExist(const int& index)
{
	return (index >= 0 && index < collision_mesh.size() && collision_mesh[index] != 0);
}

ColliderComponent* Collisionmesh::getNodeAtPosition(const int& index)
{
	if (index >= 0 && index < collision_mesh.size() && collision_mesh[index] != 0)
	{
		return mesh_nodes[(collision_mesh[index])];
	}
	return nullptr;
}

void Collisionmesh::addNode(ColliderComponent* comp)
{
	mesh_nodes.insert(std::make_pair(++mesh_index, comp));
}

void Collisionmesh::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale)
{
	cols = sX;
	rows = sY;
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
				manager.addTransformComponent(tileCol, x * tileSizeX, y * tileSizeY, tileSizeX, tileSizeY, scale);
				manager.addStaticColliderComponent(tileCol, "terrain", x * tileSizeX, y * tileSizeY, tileSizeX);
				manager.AddToGroup(&tileCol, Game::groupColliders);
				addNode(&tileCol.GetComponent<ColliderComponent>());
				collision_mesh.push_back(mesh_index);
			}
			else
			{
				collision_mesh.push_back(0);
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

	int index = manager.index_dynamic_coll;
	for (auto& dynCol : manager.compDynamicColl)
	{
		if (index-- <= 0) { break; }

		if (dynCol.entity->HasComponent<PathfindingComponent>())
		{
			// determine node position
			x = dynCol.transform->position.x / tileSizeX;
			y = dynCol.transform->position.y / tileSizeY;

			// check collision with static colliders
			force.Zero();
			bool staticCol = false;

			for (auto *n : getRegion(x, y))
			{
				if (n != nullptr)
				{
					if (Collision::AABB(dynCol, *n))
					{
						force += Collision::CalculateOpposingForce(dynCol, *n);
					}
				}
			}

			dynCol.transform->addCollisionResponseStatic(force.Normalize());
			force.Zero();
			// check collision with other agents
			if (!staticCol)
			{
				int other_index = manager.index_dynamic_coll;
				for (auto& dynCol2 : manager.compDynamicColl)
				{
					if (other_index-- <= 0) { break; }
					if (dynCol.entity != dynCol2.entity)
					{
						if (Collision::AABB(dynCol, dynCol2))
						{
							force += Collision::CalculateOpposingForce(dynCol, dynCol2);
						}
					}
				}
				dynCol.transform->addCollisionResponseDynamic(force.Normalize());
			}
		}
		
	}

	for (auto& entity : manager.GetGroup(Game::groupPlayers))
	{
		auto& dynCol = entity->GetComponent<ColliderComponent>();
		// determine node position
		x = dynCol.transform->position.x / tileSizeX;
		y = dynCol.transform->position.y / tileSizeY;

		// check collision with static colliders
		force.Zero();
		bool staticCol = false;

		for (auto *n : getRegion(x, y))
		{
			if (n != nullptr)
			{
				if (Collision::AABB(dynCol, *n))
				{
					force += Collision::CalculateOpposingForce(dynCol, *n);
				}
			}
		}

		dynCol.transform->addCollisionResponseStatic(force);
		force.Zero();

		// check collision with other agents
		if (!staticCol)
		{
			int other_index = manager.index_dynamic_coll;
			for (auto& dynCol2 : manager.compDynamicColl)
			{
				if (other_index-- <= 0) { break; }
				if (dynCol.entity != dynCol2.entity)
				{
					if (Collision::AABB(dynCol, dynCol2))
					{
						if (dynCol2.entity->HasComponent<PathfindingComponent>())
						{
							dynCol2.entity->GetComponent<TransformComponent>().addCollisionResponseDynamic(Collision::CalculateOpposingForce(dynCol2, dynCol));
						}
						else
						{
							force += Collision::CalculateOpposingForce(dynCol, dynCol2);
						}
					}
				}
			}
			dynCol.transform->addCollisionResponseDynamic(force);
		}
	}
}
