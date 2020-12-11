#pragma once
#include "SDL.h"

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

	void Update(int xpos, int ypos)
	{
		camRect.x = xpos;
		camRect.y = ypos;
		if (camRect.x < 0)
		{
			camRect.x = 0;
		}
		if (camRect.y < 0)
		{
			camRect.y = 0;
		}
		if (camRect.x > camRect.w)
		{
			camRect.x = camRect.w;
		}
		if (camRect.y > camRect.h)
		{
			camRect.y = camRect.h;
		}
	}
	int getX()
	{
		return camRect.x;
	}
	int getY()
	{
		return camRect.y;
	}

private:
	SDL_Rect camRect;
};