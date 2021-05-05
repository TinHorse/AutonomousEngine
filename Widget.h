#pragma once
#include "SDL.h"
#include "TextureManager.h"

struct Widget
{
	Widget(SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int x, int y, int w, int h, bool shouldDisplayText, bool shouldDrawTexture = true)
	{
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcW;
		srcRect.h = srcH;

		destRect.x = x;
		destRect.y = y;
		destRect.w = w;
		destRect.h = h;

		textRect.x = x + (w / 10);
		textRect.y = y + (h / 10);
		textRect.w = w / 8;
		textRect.h = h / 1.75;

		bShouldDisplayText = shouldDisplayText;
		bShouldDrawTexture = shouldDrawTexture;

		UpdateText(renderer, Text);
	}

	~Widget()
	{
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
	}

	void setTexture(std::string tID)
	{
		BoxTexture = Game::assets->GetTexture(tID);
	}

	void UpdateText(SDL_Renderer* renderer, std::string text)
	{
		if (bShouldDisplayText)
		{
			Text = text;
			switch (text.size())
			{
			case 1:
				textRect.w = destRect.w / 8;
				break;
			case 2:
				textRect.w = destRect.w / 4;
				break;
			case 3:
				textRect.w = destRect.w / 2.6666f;
				break;
			case 4:
				textRect.w = destRect.w / 2;
				break;
			}

			surfaceMessage = TTF_RenderText_Solid(Game::assets->GetFont(), Text.c_str(), *Game::assets->GetColorWhite());
			Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		}
	}

	void Draw()
	{
		// Draw Box if it should
		if (bShouldDrawTexture)
		{
			TextureManager::Draw(BoxTexture, srcRect, destRect, SDL_FLIP_NONE, 0, 0, alpha);
		}

		// Draw Text if it should
		if (bShouldDisplayText)
		{
			TextureManager::DrawText(Message, textRect);
		}
	}

	void SetAlpha(float Alpha)
	{
		alpha = Alpha;
	}

	virtual void UpdateCollision(int mouseX, int mouseY) {}

	// Box
	float alpha = 255.f;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Texture* BoxTexture;

	// Text
	std::string Text = "None";
	SDL_Rect textRect;

	bool bShouldDisplayText = true;
	bool bShouldDrawTexture = true;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
};