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
	std::map<std::string, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string tID, bool isAnimated)
	{
		animated = isAnimated;
		setTexture(tID);
	}
	
	void addAnimation(std::string name, int ind, int nFrames, int mSpeed)
	{
		Animation anim = Animation(ind, nFrames, mSpeed);
		animations.emplace(name, anim);
		Play(name);
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
		LinkComponentPointers(); // the sprite component gets a pointer to the transform component of the entity, thereby fixing its transform to the entity

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

	void Play(std::string animName)
	{
		numFrames = animations[animName].numFrames;
		animationIndex = animations[animName].index;
		animationSpeed = animations[animName].animationSpeed;
	}

	void PlayAnim(std::string anim)
	{
		if (!anim.empty())
		{
			Play(anim);
		}
		else
		{
			if (transform->getVelocity().x > 0)
			{
				spriteFlip = SDL_FLIP_NONE;
				Play("walk");
			}
			else if (transform->getVelocity().x < 0)
			{
				spriteFlip = SDL_FLIP_HORIZONTAL;
				Play("walk");
			}
			if (transform->getVelocity().y == 0)
			{
				Play("idle");
			}
		}
	}

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}


private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;


	bool animated = false;
	int numFrames = 0;
	int animationSpeed = 100;
};

