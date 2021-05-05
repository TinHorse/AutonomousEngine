#pragma once
#include "SDL.h"
#include "ECS.h"
#include "Vector2D.h"
#include "Game.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D collision_response;
	Vector2D centre;
	SDL_Point centrePt;
	float speed = 0.f;
	float top_speed = 1.5f;
	bool dynamic = false;

	float height = 128;
	float width = 128;
	float scale = 1.f;
	float initial_scale = 1.f;
	float angle = 0.f;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc, bool dyn)
	{
		position.x = 500;
		position.y = 500;
		initial_scale = scale = sc;
		dynamic = dyn;
	}

	TransformComponent(float x, float y, bool dyn)
	{
		position.x = x;
		position.y = y;
		dynamic = dyn;
	}

	TransformComponent(float x, float y, int w, int h, float sc, bool dyn, int _angle = 0, float _speed = 0.f)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		initial_scale = scale = sc;
		dynamic = dyn;
		angle = _angle;
		speed = _speed;
	}

	void Init() override
	{
		centre.x = position.x + ((width * scale) / 2);
		centre.y = position.y + ((height * scale) / 2);

		centrePt.x = centre.x;
		centrePt.y = centre.y;

	}

	void Update() override
	{
		position.x -= Game::camera.getX();
		position.y -= Game::camera.getY();

		if (dynamic)
		{
			position += collision_response;
			collision_response.Zero();
			
			centre.x = position.x + ((width * scale) / 2);
			centre.y = position.y + ((height * scale) / 2);
			centrePt.x = centre.x;
			centrePt.y = centre.y;
			
			if (speed < 0.1f) speed = 0;
			Vector2D direction = rotate_point(0, 0, toRad(angle), Vector2D(0,-1.f));
			position += direction * speed;
		}
	}

	void addCollisionResponse(const Vector2D& force)
	{
		collision_response += force;
	}

	void SetViewTarget(Vector2D target)
	{
		angle = toDeg(angleToYAxisRad(target - position));
	}

};