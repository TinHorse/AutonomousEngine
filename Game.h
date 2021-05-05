#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Camera.h"

class ColliderComponent;
class AssetManager; // forward declaration to avoid circular inclusion
class ConstructionManager;
class TurretManager;
class NodeSystem;
class Player;
class PlayerInterface;
class ManufacturingManager;

static enum BuildingType : std::size_t
{
	NoBuilding,
	Base,
	Gaslight,
	Syphon,
	Harvester,
	Scraper,
	Smithy,
	GatheringStation,
	Growhouse,
	CuttingRig,
	Smelter,
	Diffuser,
	Wiringworks,
	Hangar,
	Accelerator,
	Lighthouse,
	MachineFactory,
	COUNT
};

static enum ResourceType
{
	None,
	Mushrooms,
	Amber,
	Iron,
	Silicate,
	Obsidian,
	Morphite
};

static enum TurretType : std::size_t
{
	tNone,
	tTransporter
};

static enum EnvironmentType : std::size_t
{
	NoEnvironment,
	eWetland,
	eThorns,
	eScuffs,
	eStones,
	eFlowers
};

struct ResourceHeap
{
	ResourceHeap() = default;
	ResourceHeap(int Mushrooms, int Amber, int Iron, int Silicate, int Obsidian, int Morphite)
	{
		rMushrooms = Mushrooms;
		rAmber = Amber;
		rIron = Iron;
		rSilicate = Silicate;
		rObsidian = Obsidian;
		rMorphite = Morphite;
	}

	int rMushrooms = 0;
	int rAmber = 0;
	int rIron = 0;
	int rSilicate = 0;
	int rObsidian = 0;
	int rMorphite = 0;

	const int GetResourceAmount(enum ResourceType type) const
	{
		switch (type)
		{
		case ResourceType::None:
			break;
		case ResourceType::Mushrooms:
			return rMushrooms;
			break;
		case ResourceType::Amber:
			return rAmber;
			break;
		case ResourceType::Iron:
			return rIron;
			break;
		case ResourceType::Silicate:
			return rSilicate;
			break;
		case ResourceType::Obsidian:
			return rObsidian;
			break;
		case ResourceType::Morphite:
			return rMorphite;
			break;
		}
	}

	void Clear()
	{
		rMushrooms = 0;
		rAmber = 0;
		rIron = 0;
		rSilicate = 0;
		rObsidian = 0;
		rMorphite = 0;
	}

	int GetTotalResourceAmount()
	{
		return rMushrooms + rAmber + rIron + rSilicate + rObsidian + rMorphite;
	}
};

class Game
{
public:
	Game();
	~Game();

	void Init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void ExecuteQueues(double maxTime);
	void Render();
	void Clean();

	bool Running();

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;
	static Camera camera;

	static AssetManager *assets;
	static ConstructionManager* construction;
	static TurretManager* turretManager;
	static ManufacturingManager* manufacturingManager;
	static NodeSystem* node_system;

	static Player* player;

	static PlayerInterface * interface;

	enum groupLabels : std::size_t // enums allow users to define their own datatype. 
	{
		groupBuildings,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupResources,
		groupTransporters,
		groupEnvironment
	};

	static int GameTime;
private:
	SDL_Window *window;
};