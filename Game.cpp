#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "EntityManager.h"
#include "Components.h"
#include "Collision.h"
#include "AssetManager.h"
#include "NavMesh.h"
#include "Collisionmesh.h"
#include "Queues.h"

#include <fstream>
#include <chrono>
using namespace std::chrono;

float average_time = 0;

Map *map;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
EntityManager manager;

Camera Game::camera = Camera();

AssetManager *Game::assets = new AssetManager(&manager);
Navmesh navigation;
Collisionmesh collision;

PathfindingQueue pathfindingQueue;


bool Game::isRunning = false;
int Game::GameTime = 0;

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
			SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			std::cout << "renderer created \n";
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	// Load assets
	assets->AddTexture("collider", "assets/colliderTex.png");
	assets->AddTexture("food", "assets/foodItem.png");
	assets->AddTexture("terrain", "assets/water_map_2.png");
	assets->AddTexture("player", "assets/ship_one_BlueAlpha.png");
	assets->AddTexture("hunted", "assets/hunted_anim_alpha.png");
	assets->AddTexture("enemy", "assets/enemy.png");

	assets->AddTexture("enemy2", "assets/player2.png");

	// Load map
	map = new Map("terrain", 100, 0.5f);
	map->LoadMap("assets/water_map_2.txt", 40, 40);

	// Load navigation
	navigation.LoadMesh("assets/water_map2_collision.txt", 40, 40, 100, 100, 0.5f);

	// Load collision
	collision.LoadMesh("assets/water_map2_collision.txt", 40, 40, 100, 100, 0.5f);

	// Create player
	player = &assets->CreatePlayer(Vector2D(225,200), 85, 205, 0.3f);

	// Initialize camera
	auto& t = player->GetComponent<TransformComponent>();
	camera.Init(t.position.x, t.position.y, 2000, 2000);

	// Create Hunted
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			assets->CreateHunted(Vector2D(400+i * 40, 400+j * 40), 85, 205, 0.2f);
		}
	}
	
	// Create Food
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			assets->CreateFood(Vector2D(rand_float(100,400), rand_float(500, 800)), 407, 451, 0.12f);
		}
	}

	// Create Predators
	for (int i = 0; i < 0; i++)
	{
		for (int j = 0; j < 0; j++)
		{
			assets->CreatePredator(Vector2D(600 + i * 60, 800 + j * 60), 236, 233, 0.18f);
		}
	}
}

auto& players(manager.GetGroup(Game::groupPlayers));
auto& colliders(manager.GetGroup(Game::groupColliders));
auto& tiles(manager.GetGroup(Game::groupTiles));
auto& hunted(manager.GetGroup(Game::groupHunted));
auto& foods(manager.GetGroup(Game::groupFood));
auto& predators(manager.GetGroup(Game::groupPredators));

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
	GameTime++; // increment game time

	
	
	manager.Refresh();
	
	manager.Update();

	for (auto* h : hunted)
	{
		h->update();
	}
	for (auto* f : foods)
	{
		f->update();
	}
	for (auto* p : predators)
	{
		p->update();
	}


	Vector2D offset(-400, -300);
	camera.Update(offset + player->GetComponent<TransformComponent>().position);

	// Collision
	collision.update();

	

}

void Game::ExecuteQueues(double maxTime)
{
	auto start = std::chrono::high_resolution_clock().now();
	//std::cout << maxTime << " Max time" << std::endl;

	pathfindingQueue.UpdateDeletedEntities(manager.getDeletedEntities());
	if (maxTime > 0)
	{
		pathfindingQueue.executePathfindingRequests(maxTime);
	}
	auto end = std::chrono::high_resolution_clock().now();

	auto totaltime = std::chrono::duration_cast<microseconds>(end - start);
	if (totaltime.count() < 500)
	{
		average_time += totaltime.count();
	}

	//std::cout << average_time / GameTime << std::endl;
	//std::cout << totaltime.count() << std::endl;
}


void Game::Render() // note that all draw functions have to be called inside the SDL Renderer
{
	SDL_RenderClear(renderer);

	for (auto* t : tiles)
	{
		if (Math::distance(t->GetComponent<TileComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
			t->GetComponent<TileComponent>().Draw();
		}
	}
	for (auto* c : colliders)
	{
		if (Math::distance(c->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
			//c->GetComponent<ColliderComponent>().Draw();
		}
	}
	for (auto* h : hunted)
	{
		if (Math::distance(h->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
			h->GetComponent<SpriteComponent>().Draw();
		}
	}
	for (auto* f : foods)
	{
		if (Math::distance(f->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
		f->GetComponent<SpriteComponent>().Draw();
		}
	}
	for (auto* p : predators)
	{
		if (Math::distance(p->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
		p->GetComponent<SpriteComponent>().Draw();
		}
	}
	for (auto* p : players)
	{
		p->GetComponent<SpriteComponent>().Draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	delete assets;
	delete map;

	SDL_Quit();
	std::cout << "game cleaned \n";
}

bool Game::Running()
{
	return isRunning;
}
