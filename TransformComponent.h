#pragma once
#include "ECS.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	float speed = 1.5f;

	int height = 128;
	int width = 128;
	float scale = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int sc)
	{
		position.x = 500;
		position.y = 500;
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}

	void Init() override
	{
		velocity.Zero();
	}

	void Update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};