#pragma once
#include "SDL.h"

struct Camera
{
public:
	Camera(int xpos, int ypos, int width, int height)
	{
		camRect.x = xpos;
		camRect.y = ypos;
		camRect.w = width;
		camRect.h = height;
	}
	SDL_Rect camRect;
};