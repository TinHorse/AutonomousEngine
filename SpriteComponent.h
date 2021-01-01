#pragma once
#include "ECS.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"

class SpriteComponent : public Component
{
public:
	int animationIndex = 0;
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string tID)
	{
		setTexture(tID);
	}
	SpriteComponent(std::string tID, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0,4,100);
		Animation walk = Animation(1,5,100);
		animations.emplace("idle", idle);
		animations.emplace("walk", walk);

		Play("idle");
		setTexture(tID);
	}
	~SpriteComponent()
	{
		
	}

	void setTexture(std::string tID)
	{
		texture = Game::assets->GetTexture(tID);
	}

	void Init() override
	{
		transform = &entity->GetComponent<TransformComponent>(); // the sprite component gets a pointer to the transform component of the entity, thereby fixing its transform to the entity

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void Update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
		}

		srcRect.y = animationIndex * transform->height;
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.getX(); // static cast to int, because destRect is expecting an int
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.getY();
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char *animName)
	{
		numFrames = animations[animName].numFrames;
		animationIndex = animations[animName].index;
		animationSpeed = animations[animName].animationSpeed;
	}


private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int numFrames = 0;
	int animationSpeed = 100;
};

