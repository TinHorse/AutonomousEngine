#pragma once
#include "ECS.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D collision_response_static;
	Vector2D collision_response_dynamic;
	Vector2D previous_velocity;
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

	TransformComponent(float x, float y, int w, int h, float sc)
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
		if (collision_response_static.x || collision_response_static.y)
		{
			velocity += collision_response_static;
		}
		else if (collision_response_dynamic.x || collision_response_dynamic.y)
		{
			velocity += collision_response_dynamic;
		}
		
		collision_response_static.Zero();
		collision_response_dynamic.Zero();
		
		velocity.Normalize();
		position += velocity * speed;
		previous_velocity = velocity;
		velocity.Zero();
		
	}

	void addVelocity(const Vector2D& force)
	{
		velocity += force;
	}

	void addCollisionResponseStatic(const Vector2D& force)
	{
		collision_response_static += force;
	}

	void addCollisionResponseDynamic(const Vector2D& force)
	{
		collision_response_dynamic += force;
	}

	const Vector2D& getVelocity()
	{
		return previous_velocity;
	}

};