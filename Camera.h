#pragma once
#include "SDL.h"
#include "Vector2D.h"

class Camera
{
public:
	Camera() = default;
	void Init(int xpos, int ypos, int mWidth, int mHeight)
	{
		camRect.x = xpos;
		camRect.y = ypos;
		camRect.w = mWidth;
		camRect.h = mHeight;
	}

	void Update(const Vector2D& pos)
	{
		position = pos;
		if (position.x < 0)
		{
			position.x = 0;
		}
		if (position.y < 0)
		{
			position.y = 0;
		}
		if (position.x > camRect.w)
		{
			position.x = camRect.w;
		}
		if (position.y > camRect.h)
		{
			position.y = camRect.h;
		}
	}
	int getX()
	{
		return position.x;
	}
	int getY()
	{
		return position.y;
	}
	Vector2D GetPosition()
	{
		return position;
	}

private:
	SDL_Rect camRect;
	Vector2D position;
};