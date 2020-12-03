#pragma once
#include <vector>
#include <iostream>

static int NodeID = 0;

struct Node
{
	Node() = default;
	Node(int X, int Y, bool obs) : x(X), y(Y), isObstacle(obs)
	{
		ID = NodeID++;
	}
	bool operator==(Node& node)
	{
		return this->ID == node.ID;
	}

	int x, y;
	int ID;
	bool isObstacle;
};

struct NavMesh
{
public:
	NavMesh() = default;

	void Init(int mCols, int mRows)
	{
		cols = mCols;
		rows = mRows;
		mesh = std::vector<Node*>(mCols * mRows, new Node());
	}
	Node* operator()(size_t x, size_t y)
	{
		if ((y * cols + x) < mesh.size())
		{
			return mesh[y * cols + x];
		}
		else
		{
			std::cout << y<< " " << cols <<" "<< x << " bigger than " << mesh.size();
			return nullptr;
		}
	}

	int cols, rows;
	std::vector<Node*> mesh;
};
