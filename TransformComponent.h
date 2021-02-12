#pragma once
#include "SDL.h"
#include "ECS.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D collision_response;
	Vector2D previous_velocity;
	SDL_Point centre;
	float speed = 0.f;
	float top_speed = 1.5f;
	bool dynamic = false;

	int height = 128;
	int width = 128;
	float scale = 1;
	int angle = 0;

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

		centre.x = position.x + ((width * scale) / 2);
		centre.y = position.y + ((height * scale) / 2);
	}

	void Update() override
	{
		if (dynamic)
		{
			
			position += collision_response;
			collision_response.Zero();

			/*
			velocity.Normalize();
			position += velocity;
			previous_velocity = velocity;
			velocity.Zero();
			*/
			
			centre.x = position.x + ((width * scale) / 2);
			centre.y = position.y + ((height * scale) / 2);
			
			//if (angle < -180.f) angle = 179.f;
			//else if (angle > 180.f) angle = -179.f;
			
			if (speed < 0.1f) speed = 0;
			Vector2D direction = rotate_point(0, 0, toRad(angle), Vector2D(0,-1.f));
			position += direction * speed;
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