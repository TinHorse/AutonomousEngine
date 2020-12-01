#pragma once
#include <array>
#include <vector>

template<class T, size_t cols, size_t rows>
class Matrix
{
public:
	T& operator()(size_t x, size_t y)
	{
		return data[y * cols + x];
	}



private:
	std::array<T, cols * rows> data;
};

struct Mat
{
public:
	Mat() = default;

	void Init(int mCols, int mRows)
	{
		cols = mCols;
		rows = mRows;
		mat.reserve(mCols * mRows);
	}
	int& operator()(size_t x, size_t y)
	{
		return mat[y * cols + x];
	}

	int cols, rows;
	std::vector<int> mat;
};
