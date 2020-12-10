#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Collision.h"
#include "AssetManager.h"
#include "NavigationManager.h"
#include "CollisionManager.h"
#include <chrono>
using namespace std::chrono;

Map *map;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
EntityManager manager;
Camera Game::camera = Camera(0, 0, 800, 640);

AssetManager *Game::assets = new AssetManager(&manager);
NavigationManager navigationMan;
CollisionManager collisionMan;


bool Game::isRunning = false;

Entity *player = nullptr;



Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // check if SDL was initialized
	{
		std::cout << "subsystems initialized. \n";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags); // create window
		if (window)
		{
			std::cout << "window created \n";
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255,255,255,255);
			std::cout << "renderer created \n";
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	// load assets
	assets->AddTexture("terrain", "assets/tileset.png");
	assets->AddTexture("player", "assets/player_animated.png");
	assets->AddTexture("collider", "assets/colliderTex.png");


	// load map
	map = new Map("terrain", 32, 1);
	map->LoadMap("assets/tilemap.txt", 40, 40);

	// Load navigation
	navigationMan.LoadMesh("assets/collisionmap.txt", 40, 40, 32, 32, 1);
	navigationMan.Init();

	player = &assets->CreatePlayer(Vector2D(200,200), 90, 90, 0.25f);

	// Load collision
	collisionMan.LoadMesh("assets/collisionmap.txt", 40, 40, 32, 32, 1);
	collisionMan.AddAgent(*player);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			Entity& agent = assets->CreateAgent(Vector2D(i * 20, j * 20), 90, 90, 0.25f);
			agent.GetComponent<PathfindingComponent>().FindPath(Vector2D(600-(i*20), 600-(j*20)));
			collisionMan.AddAgent(agent);
		}
	}
}

auto& players(manager.GetGroup(Game::groupPlayers));
auto& colliders(manager.GetGroup(Game::groupColliders));
auto& tiles(manager.GetGroup(Game::groupMap));
auto& agents(manager.GetGroup(Game::groupAgents));

void Game::HandleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT: // check if QUIT was called
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update()
{
	cnt++;
	manager.Refresh();
	manager.Update();
	camera.camRect.x;
	camera.camRect.x = player->GetComponent<TransformComponent>().position.x - 400;
	camera.camRect.y = player->GetComponent<TransformComponent>().position.y - 320;

	if (camera.camRect.x < 0)
	{
		camera.camRect.x = 0;
	}
	if (camera.camRect.y < 0)
	{
		camera.camRect.y = 0;
	}
	if (camera.camRect.x > camera.camRect.w)
	{
		camera.camRect.x = camera.camRect.w;
	}
	if (camera.camRect.y > camera.camRect.h)
	{
		camera.camRect.y = camera.camRect.h;
	}

	// Collision
	collisionMan.CalculateCollision();
}

void Game::Render() // note that all draw function have to be called inside the SDL Renderer
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->Draw();
	}
	for (auto& p : players)
	{
		p->Draw();
	}
	for (auto& c : colliders)
	{
		c->Draw();
	}
	for (auto& a : agents)
	{
		a->Draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "game cleaned \n";
}

bool Game::Running()
{
	return isRunning;
}
