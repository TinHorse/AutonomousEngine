#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"
#include "ConfigBuildings.h"

class TransformComponent;

class AssetManager
{
public:
	AssetManager(EntityManager* man);
	~AssetManager();

	void Init()
	{
		Font = TTF_OpenFont("assets/BebasNeue-Regular.ttf", 96);
	}

	void LoadAllTextures();

	// Object Management: Player Entities
	Entity& CreateConstructionSite(BuildingType type, Vector2D position, float scale = 0.07f, int sizeX = 452, int sizeY = 452);

	// BUILDINGS
	Entity& CreateBase(Vector2D position);
	Entity& CreateGaslight(Vector2D position);
	Entity& CreateSyphon(Vector2D position);
	Entity& CreateScraper(Vector2D position);
	Entity& CreateHarvester(Vector2D position);
	Entity& CreateGatheringStation(Vector2D position);
	Entity& CreateWiringWorks(Vector2D position);
	Entity& CreateMachineFactory(Vector2D position);
	Entity& CreateAccelerator(Vector2D position);
	Entity& CreateLighthouse(Vector2D position);
	Entity& CreateGrowhouse(Vector2D position);
	Entity& CreateSmithy(Vector2D position);
	Entity& CreateHangar(Vector2D position);
	Entity& CreateCuttingRig(Vector2D position);

	// TRANSPORTERS & TURRETS
	Entity& CreateTransporter(Vector2D position, ResourceType resourceType = None, float scale = 0.04f, int sizeX = 200, int sizeY = 400);

	// Object Management: Resources

	// INDIVIDUAL RESOURCES
	Entity& CreateMushrooms(Vector2D position, float scale = 1.f, int sizeX = 400, int sizeY = 400);
	Entity& CreateAmberTree(Vector2D position, float scale = 1.f, int sizeX = 400, int sizeY = 400);
	Entity& CreateIronDeposit(Vector2D position, float scale = 1.f, int sizeX = 400, int sizeY = 400);
	Entity& CreateSilicateDeposit(Vector2D position, float scale = 1.f, int sizeX = 800, int sizeY = 800);
	Entity& CreateObsidianDeposit(Vector2D position, float scale = 1.f, int sizeX = 800, int sizeY = 800);

	// RESOURCE FIELDS
	void MakeAmberForest(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 400, int sizeY = 400);
	void MakeIronField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 400, int sizeY = 400);
	void MakeSilicateField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 800, int sizeY = 800);
	void MakeMushroomField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 400, int sizeY = 400);
	void MakeObsidianField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 400, int sizeY = 400);

	// Object Management: Environment

	// INDIVIDUAL ENVIRONMENT ENTITIES
	Entity& CreateWetlands(Vector2D position, float scale = 1.f, int sizeX = 712, int sizeY = 712);
	Entity& CreateThorns(Vector2D position, float scale = 1.f, int sizeX = 712, int sizeY = 712);
	Entity& CreateScuffs(Vector2D position, float scale = 1.f, int sizeX = 712, int sizeY = 712);
	Entity& CreateStones(Vector2D position, float scale = 1.f, int sizeX = 395, int sizeY = 397);
	Entity& CreateFlowers(Vector2D position, float scale = 1.f, int sizeX = 400, int sizeY = 200);

	// ENVIRONMENT FIELDS
	void MakeWetlandField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 712, int sizeY = 712);
	void MakeScuffsField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX = 712, int sizeY = 712);


	// Texture Management
	void AddTexture(std::string texID, const char *path);
	SDL_Texture *GetTexture(std::string texID);
	
	TTF_Font* GetFont()
	{
		return Font;
	}

	SDL_Color* GetColorWhite()
	{
		return &White;
	}

private:
	EntityManager *manager;
	std::map<std::string, SDL_Texture*> textures;

	std::vector<ColliderComponent> colliderComponents = std::vector<ColliderComponent>(1000);
	int collIndex = 0;

	// Interface
	TTF_Font* Font = nullptr;

	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	ConfigBuildings configB;
};