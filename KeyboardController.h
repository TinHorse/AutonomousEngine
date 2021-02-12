#pragma once
#include "Game.h"
#include "ECS.h"


class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	enum Key : std::size_t
	{
		idle,
		walk_up,
		walk_right,
		walk_down,
		walk_left,
		interact
	};

	void Init() override
	{
		LinkComponentPointers();
	}
	void Update() override
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		currentKey = idle;
		if (keystate)
		{
			if (keystate[SDL_SCANCODE_W])
			{
				if (transform->speed < transform->top_speed)
				{
					transform->speed += 0.1f;
					//currentKey = walk_up;
					//sprite->Play("walk");
				}
			}
			if (keystate[SDL_SCANCODE_S]) {
				if (transform->speed > 0)
				{
					transform->speed -= 0.1f;
					//currentKey = walk_down;
				}
			}
			if (keystate[SDL_SCANCODE_A])
			{
				transform->angle--;
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}
			if (keystate[SDL_SCANCODE_D])
			{
				transform->angle++;
				sprite->spriteFlip = SDL_FLIP_NONE;
			}
			if (keystate[SDL_SCANCODE_SPACE])
			{
				//currentKey = interact;
				float angle = entity->GetComponent<TransformComponent>().angle;
				Game::assets->CreateCannonBall(Vector2D(transform->centre.x, transform->centre.y), angle + 90, 4.f, 5, 10, 1.f);
			}

		}
		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				//sprite->Play("idle");
				break;
			case SDLK_s:
				break;
			case SDLK_a:
				break;
			case SDLK_d:
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}
	}
	
	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent <TransformComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
	}

	const Key& current()
	{
		return currentKey;
	}

private:
	Key currentKey;
};