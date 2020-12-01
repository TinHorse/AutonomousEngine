#include "Navigation.h"
#include <fstream>

Navigation::Navigation()
{
	
}

Navigation::~Navigation()
{
}

void Navigation::LoadNavMesh(const char * path, int sX, int sY)
{
	navMesh.mat = std::vector<int>(sX*sY);

	char c;
	std::fstream stream;
	stream.open(path);
	for (int y = 0; y < sY; ++y)
	{
		for (int x = 0; x < sY; ++x)
		{
			stream.get(c);
			if (c == '1')
			{
				navMesh(x, y) = atoi(&c);
			}
			stream.ignore();
		}
	}
	stream.close();
}

