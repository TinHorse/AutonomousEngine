#pragma once
#include <vector>

static int NodeID = 0;

struct Node
{
	Node() = default;
	Node(int X, int Y) : x(X), y(Y)
	{
		ID = NodeID++;
	}
	int ID;
	bool operator==(Node& node)
	{
		return this->ID == node.ID;
	}
	int x, y;
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
		return mesh[y * cols + x];
	}

	int cols, rows;
	std::vector<Node*> mesh;
};
