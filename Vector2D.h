#pragma once
#include <iostream>

class Vector2D
{
public:
	float x, y;
	Vector2D();
	Vector2D(float x, float y);

	// v1.Add(v2);
	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);
	
	// v1 = v1 + v2;
	// NOTE: 
	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	// v1 += v2;
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	// v1 = v2
	Vector2D& operator=(const Vector2D vec);

	// v1 * int
	Vector2D& operator*(const float& f);
	Vector2D& operator*(const int& i);

	Vector2D& operator+(const float& f);

	Vector2D& Zero();
	Vector2D& Normalize();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};