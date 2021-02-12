#include "CollisionMesh.h"
#include "Collision.h"
#include <fstream>
#include "EntityManager.h"

extern EntityManager manager;

void ColCell::addCollider(ColliderComponent * collider)
{
	colliders.push_back(collider);
}

std::vector<ColCell*>& ColCell::getRegion()
{
	return region;
}

void Collisionmesh::update()
{
	for (auto& v : cells)
	{
		for (auto& v2 : v)
		{
			v2.colliders.clear();
		}
	}
}

void Collisionmesh::LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, float scale)
{
	float scalar = 2.f;

	cols = sX / scalar;
	rows = sY / scalar;

	cells = std::vector<std::vector<ColCell>>(rows, std::vector<ColCell>(cols));
	// watch out for x,y
	
	tileSizeX = sTileX * scale * scalar;
	tileSizeY = sTileY * scale * scalar;

	char c;
	std::fstream stream;
	stream.open(path);

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			getCell(x, y) = ColCell(x, y, x * tileSizeX, y * tileSizeY, tileSizeX, tileSizeY);
		}
	}

	for (auto& v : cells)
	{
		for (auto& v2 : v)
		{
			auto r = getRegion(v2.indexX, v2.indexY);
			v2.region.insert(v2.region.end(),r.begin(), r.end());
		}
	}

	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sX; ++x)
		{
			stream.get(c);
			if (c == '1')
			{
				auto& tileCol(manager.AddEntity());
				manager.addTransformComponent(tileCol, x * (tileSizeX / scalar), y * (tileSizeY / scalar), tileSizeX / scalar, tileSizeY / scalar, scale, false);
				manager.addDynamicColliderComponent(tileCol, "terrain", false);

				manager.AddToGroup(&tileCol, Game::groupColliders);
			}
			stream.ignore();
		}
	}
	stream.close();
}

bool Collisionmesh::checkBounds(const int & x, const int & y)
{
	return (x >= 0 && y >= 0 && x < rows && y < cols);
}

std::vector<ColCell*> Collisionmesh::getRegion(int x, int y)
{
	std::vector<ColCell*> region;
	region.push_back(&cells[x][y]);
	if (checkBounds(x + 1, y))
		region.push_back(&cells[x + 1][y]);
	if (checkBounds(x + 1, y + 1))
		region.push_back(&cells[x + 1][y + 1]);
	if (checkBounds(x, y + 1))
		region.push_back(&cells[x][y + 1]);
	if (checkBounds(x - 1, y + 1))
		region.push_back(&cells[x - 1][y + 1]);

	return region;
}

void Collisionmesh::registerCollider(ColliderComponent * col)
{
	int x = col->transform->position.x;
	int y = col->transform->position.y;
	//int w = col->transform->width;
	//int h = col->transform->height;

	if (checkBounds(x / tileSizeX, y / tileSizeY))
	{
		auto& cell = getCell(x / tileSizeX, y / tileSizeY);
		cell.addCollider(col);
		col->current_cell = &cell;
	}

	/*
	if (checkBounds((x + w) / tileSizeX, y / tileSizeY))
	{
		auto& cell = getCell((x + w) / tileSizeX, y / tileSizeY);
		cell.addCollider(col);
	}

	if (checkBounds(x / tileSizeX, (y + h) / tileSizeY))
	{
		auto& cell = getCell(x / tileSizeX, (y + h) / tileSizeY);
		cell.addCollider(col);
	}

	if (checkBounds((x + w) / tileSizeX, (y + h) / tileSizeY))
	{
		auto& cell = getCell((x + w) / tileSizeX, (y + h) / tileSizeY);
		cell.addCollider(col);
	}
	*/
}

