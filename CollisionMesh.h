#pragma once
#include <array>
#include <set>
#include <Vector>
#include "ECS.h"
#include "SDL.h"

class ColliderComponent;

struct ColCell
{
	ColCell() {}
	ColCell(int indX, int indY, int x, int y, int w, int h)
	{
		indexX = indX;
		indexY = indY;

		bounds.x = x-1;
		bounds.y = y-1;
		bounds.w = w+2;
		bounds.h = h+2;
	}

	bool isEmpty()
	{
		return colliders.empty();
	}

	SDL_Rect bounds;
	int indexX, indexY;

	void addCollider(ColliderComponent * collider);
	std::vector<ColCell*>& getRegion();

	std::vector<ColCell*> region;
	std::vector<ColliderComponent*> colliders;
};

struct Collisionmesh
{
public:
	Collisionmesh() = default;

	void update();
	void LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale);

	ColCell& getCell(int x, int y)
	{
		return cells[x][y];
	}

	bool checkBounds(const int& x, const int& y);
	std::vector<ColCell*> getRegion(int x, int y);
	void registerCollider(ColliderComponent * col);

private:
	int cols, rows;
	int tileSizeX, tileSizeY;
	static int mesh_index;
	int index;

	//std::vector<int> collision_mesh;
	//std::unordered_map<int, ColliderComponent*> mesh_nodes;
	//std::vector<ColliderComponent*> mesh_neighbours;

	std::vector<std::vector<ColCell>> cells;
};
