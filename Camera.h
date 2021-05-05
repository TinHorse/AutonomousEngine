#pragma once
#include "SDL.h"
#include "Vector2D.h"

class Camera
{
public:
	Camera() = default;
	void Init(float xpos, float ypos, float mWidth, float mHeight)
	{
		camRect.x = xpos;
		camRect.y = ypos;
		camRect.w = mWidth;
		camRect.h = mHeight;
	}

	void Update()
	{
		previousZoomScale = globalZoomScale;
		centre.x = position.x + (camRect.w / 2);
		centre.y = position.y + (camRect.h / 2);
	}

	void Reset()
	{
		position.x = 0;
		position.y = 0;

		Stop();
	}
	float getX()
	{
		return position.x;
	}
	float getY()
	{
		return position.y;
	}
	Vector2D GetPosition()
	{
		return position;
	}

	void Stop()
	{
		/*
		if (!keyA && !keyS && !keyW && !keyD)
		{
			speed = 0.f;
			acceleration = base_acceleration;
		}
		*/
	}

	void ChangeX(float X)
	{
		//speed += acceleration;
		//speed = speed > max_speed ? max_speed : speed;
		position.x += speed * X;
	}

	void ChangeY(float Y)
	{
		//speed += acceleration;
		//speed = speed > max_speed ? max_speed : speed;
		position.y += speed * Y;
	}

	bool keyW;
	bool keyS;
	bool keyA;
	bool keyD;

	float GetGlobalScale()
	{
		return globalZoomScale;
	}
	
	void ChangeGlobalScale(float sc)
	{
		globalZoomScale += sc;
	}

	float GetZoomScaleDiff()
	{
		return globalZoomScale - previousZoomScale;
	}

	Vector2D centre = {};
private:
	SDL_Rect camRect;
	Vector2D position;
	float acceleration = 0.1f;
	float base_acceleration = 0.05f;
	float speed = 45.f;
	float max_speed = 45.f;
	float globalZoomScale = 1.f;
	float previousZoomScale = 1.f;
};