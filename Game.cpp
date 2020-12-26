#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Collision.h"
#include "AssetManager.h"
#include "NavMesh.h"
#include "Collisionmesh.h"
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
	assets->AddTexture("terrain", "assets/tileset.png");
	assets->AddTexture("player", "assets/player_animated.png");
	assets->AddTexture("collider", "assets/colliderTex.png");


	// Load map
	map = new Map("terrain", 32, 1);
	map->LoadMap("assets/tilemap.txt", 40, 40);

	// Load navigation
	navigation.LoadMesh("assets/collisionmap.txt", 40, 40, 32, 32, 1);

	player = &assets->CreatePlayer(Vector2D(225,200), 90, 90, 0.25f);

	// Load collision
	collision.LoadMesh("assets/collisionmap.txt", 40, 40, 32, 32, 1);

	// Initialize camera
	auto& t = player->GetComponent<TransformComponent>();
	camera.Init(t.position.x, t.position.y, 800, 600);

	// Create Agents
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			Entity& agent = assets->CreateAgent(Vector2D(300+i * 40, 300+j * 40), 90, 90, 0.25f);
			//agent.GetComponent<PathfindingComponent>().FindPath(Vector2D(600,600));
		}
	}
}

auto& players(manager.GetGroup(Game::groupPlayers));
auto& colliders(manager.GetGroup(Game::groupColliders));
auto& tiles(manager.GetGroup(Game::groupTiles));
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
	auto start = std::chrono::high_resolution_clock().now();
	manager.Update();
	auto end = std::chrono::high_resolution_clock().now();

	Vector2D offset(-400, -300);
	camera.Update(offset + player->GetComponent<TransformComponent>().position);
	
	// Collision
	
	collision.CalculateCollision();
	auto totaltime = std::chrono::duration_cast<milliseconds>(end - start);
	if (totaltime.count() < 500)
	{
		//average_time += totaltime.count();
	}
	//std::cout << average_time / cnt << std::endl;
}

void Game::Render() // note that all draw function have to be called inside the SDL Renderer
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		//if (Math::distance(t->GetComponent<TileComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		//{
			t->Draw();
		//}
	}
	for (auto& p : players)
	{
		p->Draw();
	}
	for (auto& c : colliders)
	{
		//if (Math::distance(c->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		//{
			c->Draw();
		//}
	}
	for (auto& a : agents)
	{
		//if (Math::distance(a->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		//{
			a->Draw();
		//}
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
