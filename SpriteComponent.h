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
	int alpha = 255;

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

	void SetTempTexture(std::string tID, int w, int h, float scale)
	{
		texture = Game::assets->GetTexture(tID);
		tempSrcRect.x = 0;
		tempSrcRect.y = 0;
		tempSrcRect.w = w;
		tempSrcRect.h = h;

		tempDestRect.x = transform->position.x;
		tempDestRect.y = transform->position.y;

		tempDestRect.x -= tempDestRect.w / 2;
		tempDestRect.y -= tempDestRect.h / 2;

		tempDestRect.w = w * scale;
		tempDestRect.h = h * scale;
		bUseTempSize = true;
	}

	void DisableTempTexture()
	{
		bUseTempSize = false;
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

		if (bUseTempSize)
		{
			tempDestRect.x = static_cast<int>(transform->position.x);
			tempDestRect.y = static_cast<int>(transform->position.y);

			// Centre the sprite
			tempDestRect.x -= tempDestRect.w / 2;
			tempDestRect.y -= tempDestRect.h / 2;
			return;
		}
		
		srcRect.y = animationIndex * transform->height;
		destRect.x = static_cast<int>(transform->position.x); // static cast to int, because destRect is expecting an int
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;

		// Centre the sprite
		destRect.x -= destRect.w / 2;
		destRect.y -= destRect.h / 2;
	}

	void Draw() override
	{
		if (bHidden) return;
		if (bUseTempSize)
		{
			TextureManager::Draw(texture, tempSrcRect, tempDestRect, spriteFlip, transform->angle, &transform->centrePt, alpha);
			return;
		}
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip, transform->angle, &transform->centrePt, alpha);
	}

	void Play(std::string animName)
	{
		numFrames = animations[animName].numFrames;
		animationIndex = animations[animName].index;
		animationSpeed = animations[animName].animationSpeed;
	}

	void PlayAnim(std::string anim, long int ticks)
	{
		/*
		if (ticks % 10 == 0)
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
				else if (transform->getVelocity().y == 0)
				{
					Play("idle");
				}
				
			}
		}
		*/
	}

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Hide()
	{
		bHidden = true;
	}

	void UnHide()
	{
		bHidden = false;
	}

private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;
	SDL_Rect tempSrcRect;
	SDL_Rect tempDestRect;
	bool bUseTempSize = false;

	bool animated = false;
	int numFrames = 0;
	int animationSpeed = 100;

	bool bHidden = false;
};

