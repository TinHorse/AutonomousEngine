#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D & Vector2D::Add(const Vector2D & vec)
{
	this->x += vec.x; // "this" represents the owning object
	this->y += vec.y;
	return *this; // return the resulting vector
}

Vector2D & Vector2D::Subtract(const Vector2D & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

Vector2D & Vector2D::Multiply(const Vector2D & vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

Vector2D & Vector2D::Divide(const Vector2D & vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

Vector2D & operator+(Vector2D& v1, const Vector2D & v2)
{
	return v1.Add(v2); // we already have an add method defined, so we can use it here
}

Vector2D & operator-(Vector2D& v1, const Vector2D & v2)
{
	return v1.Subtract(v2);
}

Vector2D & operator*(Vector2D& v1, const Vector2D & v2)
{
	return v1.Multiply(v2);
}

Vector2D & operator/(Vector2D& v1, const Vector2D & v2)
{
	return v1.Divide(v2);
}

std::ostream & operator<<(std::ostream & stream, const Vector2D & vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

Vector2D & Vector2D::operator+=(const Vector2D & vec)
{
	return this->Add(vec); // use previously defined method and add to "this"
}

Vector2D & Vector2D::operator-=(const Vector2D & vec)
{
	return this->Subtract(vec);
}

Vector2D & Vector2D::operator*=(const Vector2D & vec)
{
	return this->Multiply(vec);
}

Vector2D & Vector2D::operator/=(const Vector2D & vec)
{
	return this->Divide(vec);
}

Vector2D & Vector2D::operator=(const Vector2D& vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;
}

Vector2D & Vector2D::operator*(const float & f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

Vector2D & Vector2D::operator*(const int & i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}

Vector2D & Vector2D::operator+(const float & f)
{
	this->x += f;
	this->y += f;
	return *this;
}

Vector2D & Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

Vector2D & Vector2D::Normalize()
{
	float mag = sqrt((this->x * this->x) + (this->y * this->y));
	if (mag != 0)
	{
		this->x /= mag;
		this->y /= mag;
	}
	else
	{
		this->Zero();
	}
	return *this;
}
