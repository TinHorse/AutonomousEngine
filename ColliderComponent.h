#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

// The collider component is used to define a collision rectangle that checks for collisions with other rectangles

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	TransformComponent *transform;

	ColliderComponent(std::string mTag)
	{
		tag = mTag;
	}
	ColliderComponent(std::string mTag, int xpos,  int ypos, int size)
	{
		tag = mTag;
		collider.x = xpos;
		collider.y = ypos;
		collider.w = collider.h = size;
	}

	void Init() override
	{
		if (!entity->HasComponent<TransformComponent>())
		{
			entity->AddComponent<TransformComponent>();
		}
		transform = &entity->GetComponent<TransformComponent>();

		texture = TextureManager::LoadTexture("assets/colliderTex.png");
		srcRect = { 0,0,320,320 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void Update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destRect.x = collider.x - Game::camera.camRect.x;
		destRect.y = collider.y - Game::camera.camRect.y;
	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

};

