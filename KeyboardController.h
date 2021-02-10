#pragma once
//#include "Game.h"
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
		transform->velocity.Zero();
		currentKey = idle;
		if (keystate)
		{
			if (keystate[SDL_SCANCODE_W])
			{
				transform->velocity.y = -1;
				currentKey = walk_up;
				//sprite->Play("walk");
			}
			if (keystate[SDL_SCANCODE_S]) {
				transform->velocity.y = 1;
				currentKey = walk_down;
				//sprite->Play("walk");
			}
			if (keystate[SDL_SCANCODE_A])
			{
				transform->velocity.x = -1;
				currentKey = walk_left;
				//sprite->Play("walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}
			if (keystate[SDL_SCANCODE_D])
			{
				transform->velocity.x = 1;
				currentKey = walk_right;
				//sprite->Play("walk");
				sprite->spriteFlip = SDL_FLIP_NONE;
			}
			if (keystate[SDL_SCANCODE_SPACE])
			{
				currentKey = interact;
				//sprite->Play("eat");
			}

		}
		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				if (!transform->velocity.x)
				{
					sprite->Play("idle");
				}
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				if (!transform->velocity.x)
				{
					sprite->Play("idle");
				}
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				if (!transform->velocity.y)
				{
					sprite->Play("idle");
				}
				//if(sprite->spriteFlip = SDL_FLIP_NONE)
				//sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				if (!transform->velocity.y)
				{
					sprite->Play("idle");
				}
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