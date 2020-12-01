#pragma once
#include "Matrix.h"

class Navigation
{
public:
	Navigation();
	~Navigation();

	void LoadNavMesh(const char *path, int sX, int sY);

private:
	Mat navMesh;
};
