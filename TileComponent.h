#pragma once
#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	SDL_Texture *texture;
	std::string texID;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	Vector2D position;

	TileComponent() = default;

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tscale, int tsize, std::string tID)
	{
		texture = Game::assets->GetTexture(tID);
		texID = tID;

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale;
	}

	~TileComponent()
	{
		
	}

	void Update() override
	{
		destRect.x = position.x - Game::camera.getX();
		destRect.y = position.y - Game::camera.getY();
	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
	

};


