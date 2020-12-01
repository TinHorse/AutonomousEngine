#include "TextureManager.h"

SDL_Texture * TextureManager::LoadTexture(const char *texture)
{
	SDL_Surface *tmpSurface = IMG_Load(texture); // load image
	SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface); // create a texture
	SDL_FreeSurface(tmpSurface); // free texture memory

	return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip); // render a copy of the given texture in the specified location
}
