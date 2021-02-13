#pragma once
#include "Vector2D.h"
#include "Math.h"

#include "Components.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent() = default;

	ProjectileComponent(int xpos, int ypos, float tscale, int sizeX, int sizeY, std::string tID)
	{
		texture = Game::assets->GetTexture(tID);;

		srcRect.w = sizeX;
		srcRect.h = sizeY;

		destRect.x = xpos;
		destRect.y = ypos;
	}

	void Init() override
	{
		LinkComponentPointers();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

		destRect.w = transform->width;
		destRect.h = transform->height;
	}

	void Update() override
	{
		destRect.x = transform->position.x - Game::camera.getX();
		destRect.y = transform->position.y - Game::camera.getY();


	}


	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE, transform->angle, &transform->centrePt);
	}

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
		collider = &entity->GetComponent<ColliderComponent>();
	}

private:
	TransformComponent * transform;
	ColliderComponent * collider;

	SDL_Texture *texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;

	Vector2D velocity;
};