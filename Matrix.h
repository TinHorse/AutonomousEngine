#pragma once
#include <array>
#include <vector>

class Node;

template<class T, size_t rows, size_t cols>
class Matrix
{
	std::array<T, rows * cols> m_Data;
public:
	T& operator()(size_t y, size_t x)
	{
		return m_Data[y * cols + x];
	}

	
};

struct Mat
{
public:
	Mat() = default;

	void Init(int mCols, int mRows, int defaultValue=0)
	{
		cols = mCols;
		rows = mRows;
		mat = std::vector<int>(mCols * mRows, defaultValue);
	}
	int& operator()(size_t x, size_t y)
	{
		return mat[y * cols + x];
	}

	int cols, rows;
	std::vector<int> mat;
};
