#include "Components.h"
#include "CollisionMesh.h"
using std::vector;

class CollisionManager
{
public:
	void LoadMesh(const char * path, int sX, int sY, int sTileX, int sTileY, int scale);
	void CalculateCollision();

private:
	CollisionMesh colMat;
	int tileSizeX, tileSizeY;
};