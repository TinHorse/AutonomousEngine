#pragma once
#include "Game.h"

// TextureManager doesn't have anything to store. It is only used as a container for helper functions to load, draw (etc) textures. 
// This is why we never instantiate a TextureManager object but instead use it e.g. as TextureManager::loadTexture("tex.png")

struct TextureManager
{
	static SDL_Texture *LoadTexture(const char *fileName); // load texture from file
	static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip); // draw a texture in the given rectangle
};