#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Camera.h"

class ColliderComponent;
class AssetManager; // forward declaration to avoid circular inclusion

class Game
{
public:
	Game();
	~Game();

	void Init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running();

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;
	static Camera camera;

	static AssetManager *assets;

	enum groupLabels : std::size_t // enums allow users to define their own datatype. 
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupAgents
	};

private:
	int cnt = 0;
	SDL_Window *window;
};