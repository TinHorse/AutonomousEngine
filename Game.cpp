#include "Game.h"
#include "SDL_ttf.h"
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
#include "InputManager.h"
#include "ConstructionManager.h"
#include "TurretManager.h"
#include "NodeSystem.h"
#include "Player.h"
#include "PlayerInterface.h"
#include "ManufacturingManager.h"

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

PathfindingQueue pathfindingQueue(manager);

InputManager input;

ConstructionManager* Game::construction = new ConstructionManager();
TurretManager* Game::turretManager = new TurretManager();
ManufacturingManager* Game::manufacturingManager = new ManufacturingManager();

NodeSystem* Game::node_system = new NodeSystem();


bool Game::isRunning = false;
int Game::GameTime = 0;

Player* Game::player = new Player(manager);

PlayerInterface* Game::interface = new PlayerInterface(Game::player);

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

	assets->LoadAllTextures();

	// Load navigation
	//navigation.LoadMesh("assets/water_map2_collision.txt", 40, 40, 100, 100, 0.5f);

	// Load collision
	//collision.LoadMesh("assets/water_map2_collision.txt", 40, 40, 100, 100, 0.5f);

	// Create player
	manager.addPlayerResourceComponent(*player);

	// Initialize camera
	//auto& t = player->GetComponent<TransformComponent>();
	camera.Init(0,0, 1600,880);


	// Create Base
	//assets->CreateBase(Vector2D(1000, 1000), 85, 205, 1.f);

	// Initialize fonts
	TTF_Init();

	assets->Init(); // load font

	interface->Init(renderer, construction, &player->GetComponent<PlayerResourceComponent>(), turretManager, node_system, manufacturingManager);

	for (int i = 0; i < 35; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(1, 2);
		float random_distBetweenX = rand_float(50.f, 80.f);
		float random_distBetweenY = rand_float(50.f, 80.f);
		assets->MakeMushroomField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 9; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(2, 4);
		float random_distBetweenX = rand_float(40.f, 60.f);
		float random_distBetweenY = rand_float(40.f, 60.f);
		assets->MakeObsidianField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 25; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(3, 20);
		float random_distBetweenX = rand_float(20.f, 70.f);
		float random_distBetweenY = rand_float(20.f, 70.f);
		assets->MakeAmberForest(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 22; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(3, 8);
		float random_distBetweenX = rand_float(60.f, 120.f);
		float random_distBetweenY = rand_float(60.f, 120.f);
		assets->MakeIronField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 18; i++)
	{
		Vector2D random_position = { rand_float(-300.f, 1700.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(1, 2);
		float random_distBetweenX = rand_float(20.f, 40.f);
		float random_distBetweenY = rand_float(100.f, 200.f);
		assets->MakeSilicateField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 11; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(8,15);
		float random_distBetweenX = rand_float(200.f, 250.f);
		float random_distBetweenY = rand_float(100.f, 170.f);
		assets->MakeWetlandField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

	for (int i = 0; i < 23; i++)
	{
		Vector2D random_position = { rand_float(-650.f, 2100.f), rand_float(-300.f,1200) };
		int random_amount = rand_int(8, 15);
		float random_distBetweenX = rand_float(60.f, 150.f);
		float random_distBetweenY = rand_float(60.f, 150.f);
		assets->MakeScuffsField(random_position, random_amount, random_distBetweenX, random_distBetweenY);
	}

}

auto& colliders(manager.GetGroup(Game::groupColliders));
auto& projectiles(manager.GetGroup(Game::groupProjectiles));
auto& buildings(manager.GetGroup(Game::groupBuildings));
auto& resources(manager.GetGroup(Game::groupResources));
auto& transporters(manager.GetGroup(Game::groupTransporters));
auto& environment(manager.GetGroup(Game::groupEnvironment));


void Game::HandleEvents()
{
	input.Update();

	input.HandleContinuousInput();

	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type)
	{
	case SDL_QUIT: // check if QUIT was called
		isRunning = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		input.HandleMouseButton(e.button, manager);
		break;
	case SDL_KEYDOWN:
		input.HandleKeyDown();
		break;
	case SDL_KEYUP:
		input.HandleKeyUp(e);
		break;
	case SDL_MOUSEWHEEL:
		input.HandleMouseWheel(e.wheel);
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

	camera.Update();

	construction->UpdateMousePosition(input.GetMouseX(), input.GetMouseY());
	construction->Update();
	construction->UpdateActiveConstructions(manager, GameTime);

	manufacturingManager->Update();

	interface->UpdateMousePosition(input.GetMouseX(), input.GetMouseY());

	node_system->UpdatePlayerResourcesFromStorages();
	node_system->Update(GameTime, camera.GetPosition());
	
	for (auto* e : projectiles)
	{
		e->Update();
	}

	camera.Reset();


	// Collision
	collision.update();

	interface->Update(renderer, GameTime);

	// Update construction manager
	if (construction->ShouldSumResourcesInArea())
	{
		construction->SumResourcesInArea(manager);
	}




	// UPDATE LIGHTING PROTOTYPE
	if (construction->updateLightingIntensity > 0.f)
	{
		for (auto r : resources)
		{
			TransformComponent& transform = r->GetComponent<TransformComponent>();
			float dist = Math::distance(construction->LastBuilding->GetComponent<TransformComponent>().position, transform.position);
			if (dist < 300.f)
			{
				int previousAplha = r->GetComponent<SpriteComponent>().alpha;
				int alpha = 255 - (dist * (dist/(construction->updateLightingIntensity * 3.f)));
				if (alpha < 0) alpha = 0;
				if (alpha > 255) alpha = 255;
				

				alpha = std::max(alpha, previousAplha);

				r->GetComponent<SpriteComponent>().alpha = alpha;
			}
		}
		for (auto r : environment)
		{
			TransformComponent& transform = r->GetComponent<TransformComponent>();
			float dist = Math::distance(construction->LastBuilding->GetComponent<TransformComponent>().position, transform.position);
			if (dist < 300.f)
			{
				int previousAplha = r->GetComponent<SpriteComponent>().alpha;
				int alpha = 255 - (dist * (dist / (construction->updateLightingIntensity * 3.f)));
				if (alpha < 0) alpha = 0;
				if (alpha > 255) alpha = 255;


				alpha = std::max(alpha, previousAplha);

				r->GetComponent<SpriteComponent>().alpha = alpha;
			}
		}
		for (auto r : buildings)
		{
			TransformComponent& transform = r->GetComponent<TransformComponent>();
			float dist = Math::distance(construction->LastBuilding->GetComponent<TransformComponent>().position, transform.position);
			if (dist < 300.f)
			{
				int previousAplha = r->GetComponent<SpriteComponent>().alpha;
				int alpha = 255 - (dist * (dist / (construction->updateLightingIntensity * 3.f)));
				if (alpha < 0) alpha = 0;
				if (alpha > 255) alpha = 255;


				alpha = std::max(alpha, previousAplha);

				r->GetComponent<SpriteComponent>().alpha = alpha;
			}
		}
		construction->updateLightingIntensity = 0.f;
		construction->LastBuilding = nullptr;
	}
	
}

void Game::ExecuteQueues(double maxTime)
{
	pathfindingQueue.refresh();

	if (maxTime > 0)
	{
		pathfindingQueue.executePathfindingRequests(maxTime);
	}
}


void Game::Render() // note that all draw functions have to be called inside the SDL Renderer
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	node_system->Draw(renderer);
	construction->DrawShadow(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	for (auto* e : colliders)
	{
		/*
		if (Math::distance(c->GetComponent<TransformComponent>().position, player->GetComponent<TransformComponent>().position) < 300)
		{
			c->GetComponent<ColliderComponent>().Draw();
		}
		*/
	}

	for (auto* e : environment)
	{
		e->GetComponent<SpriteComponent>().Draw();
	}

	for (auto* e : resources)
	{
		e->GetComponent<SpriteComponent>().Draw();
	}

	for (auto* e : buildings)
	{
		e->GetComponent<SpriteComponent>().Draw();
	}

	for (auto* e : transporters)
	{
		e->GetComponent<SpriteComponent>().Draw();
	}


	interface->DrawWidgets();

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
