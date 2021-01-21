#pragma once
#include "ECS.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D collision_response;
	Vector2D previous_velocity;
	float speed = 1.5f;
	bool dynamic = false;

	int height = 128;
	int width = 128;
	float scale = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int sc, bool dyn)
	{
		position.x = 500;
		position.y = 500;
		scale = sc;
		dynamic = dyn;
	}

	TransformComponent(float x, float y, bool dyn)
	{
		position.x = x;
		position.y = y;
		dynamic = dyn;
	}

	TransformComponent(float x, float y, int w, int h, float sc, bool dyn)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
		dynamic = dyn;
	}

	void Init() override
	{
		velocity.Zero();
	}

	void Update() override
	{
		if (dynamic)
		{
			float mag = 0;
			if (collision_response.x || collision_response.y)
			{
				if (!velocity.x && !velocity.y)
				{
					position += collision_response.Normalize() * (speed);
					collision_response.Zero();
					return;
				}
				else
				{
					mag = speed / 2.f;
					position += collision_response.Normalize() * (speed - mag);
				}
				collision_response.Zero();
			}
			//velocity* 0.5f;

			velocity.Normalize();
			position += velocity * (speed - mag);
			previous_velocity = velocity;
			velocity.Zero();
		}
	}

	void addVelocity(const Vector2D force)
	{
		velocity += force;
	}

	void addCollisionResponse(const Vector2D& force)
	{
		collision_response += force;
	}

	const Vector2D& getVelocity()
	{
		return previous_velocity;
	}

};