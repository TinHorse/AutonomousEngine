#pragma once
#include "Vector2D.h"
#include "Math.h"
#include "Components.h"

class AuxiliaryComponent : public Component
{
public:
	AuxiliaryComponent() = default;

	AuxiliaryComponent(int xoffset, int yoffset, float tscale, float _angle, int sizeX, int sizeY, std::string tID, TransformComponent& _transform)
	{
		texture = Game::assets->GetTexture(tID);;

		transform = &_transform;

		srcRect.w = sizeX;
		srcRect.h = sizeY;

		offset.x = xoffset;
		offset.y = yoffset;

		scale = tscale;
		angle = _angle;
	}

	void Init() override
	{
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

		destRect.w = transform->width * scale;
		destRect.h = transform->height * scale;
	}

	void Update() override
	{
		destRect.x = offset.x + transform->centre.x - Game::camera.getX();
		destRect.y = offset.y + transform->centre.y - Game::camera.getY();
	}


	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE, angle + transform->angle, &transform->centrePt);
	}

	void setFiring(bool _firing)
	{
		firing = _firing;
	}

	void setTransform(TransformComponent& _transform)
	{
		transform = &_transform;
	}

private:
	TransformComponent * transform;

	SDL_Texture *texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	Vector2D offset;
	float angle;
	float scale;

	int reload_timer;
	int max_reload;
	bool firing = false;
};