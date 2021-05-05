#include "AssetManager.h"
#include "Components.h"
#include "EntityManager.h"
#include "Entities.h"
#include "SDL_ttf.h"

extern Collisionmesh collision;

AssetManager::AssetManager(EntityManager *man) : manager(man)
{
	
}
AssetManager::~AssetManager()
{
	for (auto& t : textures)
	{
		SDL_DestroyTexture(t.second);
	}
}

void AssetManager::LoadAllTextures()
{
	AddTexture("collider", "assets/colliderTex.png");
	AddTexture("player", "assets/ship_one_noColourAlpha.png");
	AddTexture("WidgetDefault", "assets/WidgetDefault.png");
	AddTexture("ResourceColours", "assets/InterfaceResourceColours.png");

	auto& texturesB = configB.BuildingSpriteNames;

	// Buildings
	AddTexture("construction_site", "assets/ConstructionSite.png");
	AddTexture(texturesB[Gaslight], "assets/Gaslight.png");
	AddTexture(texturesB[Scraper], "assets/Scraper.png");
	AddTexture(texturesB[Syphon], "assets/Syphon.png");
	AddTexture(texturesB[Base], "assets/Base.png");
	AddTexture(texturesB[Harvester], "assets/Harvester.png");
	AddTexture(texturesB[GatheringStation], "assets/GatheringStation.png");
	AddTexture(texturesB[MachineFactory], "assets/MachineFactory.png");
	AddTexture(texturesB[Wiringworks], "assets/WiringWorks.png");
	AddTexture(texturesB[Accelerator], "assets/Accelerator.png");
	AddTexture(texturesB[Lighthouse], "assets/Lighthouse.png");
	AddTexture(texturesB[Growhouse], "assets/Growhouse.png");
	AddTexture(texturesB[Smithy], "assets/Smithy.png");
	AddTexture(texturesB[Hangar], "assets/Hangar.png");
	AddTexture(texturesB[CuttingRig], "assets/CuttingRig.png");

	// Transporters
	AddTexture("transporter_empty", "assets/TransporterEmpty.png");
	AddTexture("transporter_mushrooms", "assets/TransporterMushrooms.png");
	AddTexture("transporter_amber", "assets/TransporterAmber.png");
	AddTexture("transporter_iron", "assets/TransporterIron.png");
	AddTexture("transporter_silicate", "assets/TransporterSilicate.png");
	AddTexture("transporter_obsidian", "assets/TransporterObsidian.png");
	AddTexture("transporter_morphite", "assets/TransporterMorphite.png");

	// Mushrooms
	AddTexture("MushroomsA", "assets/Mushrooms1.png");
	AddTexture("MushroomsB", "assets/Mushrooms2.png");
	AddTexture("MushroomsC", "assets/Mushrooms3.png");
	
	// Amber
	AddTexture("AmberTreeA", "assets/AmberTree1.png");
	AddTexture("AmberTreeB", "assets/AmberTree2.png");
	AddTexture("AmberTreeC", "assets/AmberTree3.png");
	AddTexture("AmberTreeD", "assets/AmberTree4.png");
	AddTexture("AmberTreeE", "assets/AmberTree5.png");

	// Iron
	AddTexture("IronDepositA", "assets/IronDeposit1.png");
	AddTexture("IronDepositB", "assets/IronDeposit2.png");
	AddTexture("IronDepositC", "assets/IronDeposit3.png");
	AddTexture("IronDepositD", "assets/IronDeposit4.png");
	AddTexture("IronDepositE", "assets/IronDeposit5.png");
	AddTexture("IronDepositF", "assets/IronDeposit6.png");
	AddTexture("IronDepositG", "assets/IronDeposit7.png");

	// Silicate
	AddTexture("SilicateDepositA", "assets/Silicate1.png");
	AddTexture("SilicateDepositB", "assets/Silicate2.png");

	// Obsidian
	AddTexture("ObsidianDepositA", "assets/Obsidian1.png");
	AddTexture("ObsidianDepositB", "assets/Obsidian2.png");
	AddTexture("ObsidianDepositC", "assets/Obsidian3.png");
	AddTexture("ObsidianDepositD", "assets/Obsidian4.png");

	// ENVIRONMENT
	// Wetlands
	AddTexture("WetlandsA", "assets/Wetlands1.png");
	AddTexture("WetlandsB", "assets/Wetlands2.png");
	AddTexture("WetlandsC", "assets/Wetlands3.png");
	AddTexture("WetlandsD", "assets/Wetlands4.png");
	AddTexture("WetlandsE", "assets/Wetlands5.png");
	AddTexture("WetlandsF", "assets/Wetlands6.png");

	// Thorns
	AddTexture("ThornsA", "assets/Thorns1.png");
	AddTexture("ThornsB", "assets/Thorns2.png");
	AddTexture("ThornsC", "assets/Thorns3.png");
	AddTexture("ThornsD", "assets/Thorns4.png");
	AddTexture("ThornsE", "assets/Thorns5.png");

	// Scuffs
	AddTexture("ScuffsA", "assets/Scuffs1.png");
	AddTexture("ScuffsB", "assets/Scuffs2.png");
	AddTexture("ScuffsC", "assets/Scuffs3.png");
	AddTexture("ScuffsD", "assets/Scuffs4.png");
	AddTexture("ScuffsE", "assets/Scuffs5.png");

	// Stones
	AddTexture("StonesA", "assets/Stones1.png");
	AddTexture("StonesB", "assets/Stones2.png");
	AddTexture("StonesC", "assets/Stones3.png");

	// Flowers
	AddTexture("FlowersA", "assets/Flowers1.png");
	AddTexture("FlowersB", "assets/Flowers2.png");
	AddTexture("FlowersC", "assets/Flowers3.png");
	AddTexture("FlowersD", "assets/Flowers4.png");
	AddTexture("FlowersE", "assets/Flowers5.png");
}

Entity & AssetManager::CreateConstructionSite(BuildingType type, Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<ConstructionSite>(type);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	auto& sprite = manager->addSpriteComponent(entity, "construction_site", false);
	auto& storage = manager->addStorageComponent(entity);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

// ----------------------------- INFRASTRUCTURE ------------------------------------------------------------

Entity& AssetManager::CreateBase(Vector2D position)
{
	auto& size = configB.BuildingSizes[Base];
	auto& textureB = configB.BuildingSpriteNames[Base];
	Entity& entity = manager->AddEntity<Building>(Base, 400.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);

	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	available_buildings.push_back(Base);
	available_buildings.push_back(MachineFactory);
	available_buildings.push_back(Wiringworks);
	available_buildings.push_back(Smithy);
	available_buildings.push_back(Hangar);
	manager->addManufacturingComponent(entity, 2400, available_buildings, available_turrets, true);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateGaslight(Vector2D position)
{
	auto& size = configB.BuildingSizes[Gaslight];
	auto& textureB = configB.BuildingSpriteNames[Gaslight];
	Entity& entity = manager->AddEntity<Building>(Gaslight, 200.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}


Entity & AssetManager::CreateAccelerator(Vector2D position)
{
	auto& size = configB.BuildingSizes[Accelerator];
	auto& textureB = configB.BuildingSpriteNames[Accelerator];
	Entity& entity = manager->AddEntity<Building>(Accelerator, 100.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateLighthouse(Vector2D position)
{
	auto& size = configB.BuildingSizes[Lighthouse];
	auto& textureB = configB.BuildingSpriteNames[Lighthouse];
	Entity& entity = manager->AddEntity<Building>(Lighthouse, 100.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

// ----------------------------- RESOURCE BUILDINGS ------------------------------------------------------------

Entity& AssetManager::CreateSyphon(Vector2D position)
{
	auto& size = configB.BuildingSizes[Syphon];
	auto& textureB = configB.BuildingSpriteNames[Syphon];
	Entity& entity = manager->AddEntity<Building>(Syphon, 50.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);
	auto& production = manager->addProductionComponent(entity, Amber, 3, 700, Mushrooms, 3, true, true);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateScraper(Vector2D position)
{
	auto& size = configB.BuildingSizes[Scraper];
	auto& textureB = configB.BuildingSpriteNames[Scraper];
	Entity& entity = manager->AddEntity<Building>(Scraper, 80.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);
	auto& production = manager->addProductionComponent(entity, Iron, 3, 600, Mushrooms, 2, true, true);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateHarvester(Vector2D position)
{
	auto& size = configB.BuildingSizes[Harvester];
	auto& textureB = configB.BuildingSpriteNames[Harvester];
	Entity& entity = manager->AddEntity<Building>(Harvester, 25.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);
	auto& production = manager->addProductionComponent(entity, Silicate, 3, 600, Mushrooms, 2, true, true);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateGatheringStation(Vector2D position)
{
	auto& size = configB.BuildingSizes[GatheringStation];
	auto& textureB = configB.BuildingSpriteNames[GatheringStation];
	Entity& entity = manager->AddEntity<Building>(GatheringStation, 100.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);
	auto& production = manager->addProductionComponent(entity, Mushrooms, 3, 400, None, 0, true, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateGrowhouse(Vector2D position)
{
	auto& size = configB.BuildingSizes[Growhouse];
	auto& textureB = configB.BuildingSpriteNames[Growhouse];
	Entity& entity = manager->AddEntity<Building>(Growhouse, 0.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	auto& sprite = manager->addSpriteComponent(entity, textureB, false);
	auto& storage = manager->addStorageComponent(entity);
	auto& production = manager->addProductionComponent(entity, Mushrooms, 3, 400, Amber, 0, false, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateCuttingRig(Vector2D position)
{
	auto& size = configB.BuildingSizes[CuttingRig];
	auto& textureB = configB.BuildingSpriteNames[CuttingRig];
	Entity& entity = manager->AddEntity<Building>(CuttingRig, 60.f);
	manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);

	manager->addSpriteComponent(entity, textureB, false);
	manager->addStorageComponent(entity);
	manager->addProductionComponent(entity, Obsidian, 3, 1000, Mushrooms, 2, true, false);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

// ----------------------------- MANUFACTURING & HANGAR -------------------------------------------------------

Entity & AssetManager::CreateWiringWorks(Vector2D position)
{
	auto& size = configB.BuildingSizes[Wiringworks];
	auto& textureB = configB.BuildingSpriteNames[Wiringworks];
	Entity& entity = manager->AddEntity<Building>(Wiringworks, 100.f);
	
	manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	manager->addSpriteComponent(entity, textureB, false);
	manager->addStorageComponent(entity);

	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	available_buildings.push_back(Gaslight);
	available_buildings.push_back(Accelerator);
	available_buildings.push_back(Lighthouse);
	manager->addManufacturingComponent(entity, 1200, available_buildings, available_turrets);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateMachineFactory(Vector2D position)
{
	auto& size = configB.BuildingSizes[MachineFactory];
	auto& textureB = configB.BuildingSpriteNames[MachineFactory];
	Entity& entity = manager->AddEntity<Building>(MachineFactory, 150.f);
	manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	manager->addSpriteComponent(entity, textureB, false);
	manager->addStorageComponent(entity);
	
	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	available_buildings.push_back(Scraper);
	available_buildings.push_back(Harvester);
	available_buildings.push_back(CuttingRig);
	manager->addManufacturingComponent(entity, 4000, available_buildings, available_turrets);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}


Entity & AssetManager::CreateSmithy(Vector2D position)
{
	auto& size = configB.BuildingSizes[Smithy];
	auto& textureB = configB.BuildingSpriteNames[Smithy];
	Entity& entity = manager->AddEntity<Building>(Smithy, 150.f);
	 manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	manager->addSpriteComponent(entity, textureB, false);
	manager->addStorageComponent(entity);

	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	available_buildings.push_back(Syphon);
	available_buildings.push_back(GatheringStation);
	manager->addManufacturingComponent(entity, 2400, available_buildings, available_turrets);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

Entity & AssetManager::CreateHangar(Vector2D position)
{
	auto& size = configB.BuildingSizes[Hangar];
	auto& textureB = configB.BuildingSpriteNames[Hangar];
	Entity& entity = manager->AddEntity<Building>(Hangar, 150.f);
	manager->addTransformComponent(entity, position.x, position.y, size.w, size.h, size.scale, true);
	
	manager->addSpriteComponent(entity, textureB, false);
	manager->addStorageComponent(entity);

	std::vector<BuildingType> available_buildings;
	std::vector<TurretType> available_turrets;
	available_turrets.push_back(tTransporter);
	manager->addManufacturingComponent(entity, 1800, available_buildings, available_turrets);

	//manager->addHangarComponent(entity);

	manager->AddToGroup(&entity, Game::groupBuildings);
	return entity;
}

// ----------------------------- TURRETS ------------------------------------------------------------

Entity & AssetManager::CreateTransporter(Vector2D position, ResourceType resourceType, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Transporter>();
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);

	std::string TransporterType;
	switch (resourceType)
	{
	case None:
		TransporterType = "transporter_empty";
		break;
	case Mushrooms:
		TransporterType = "transporter_mushrooms";
		break;
	case Amber:
		TransporterType = "transporter_amber";
		break;
	case Iron:
		TransporterType = "transporter_iron";
		break;
	case Silicate:
		TransporterType = "transporter_silicate";
		break;
	case Obsidian:
		TransporterType = "transporter_obsidian";
		break;
	case Morphite:
		TransporterType = "transporter_morphite";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, TransporterType, false);
	auto& nodemovement = manager->addNodeMovementComponent(entity, TurretType::tTransporter);

	manager->AddToGroup(&entity, Game::groupTransporters);
	return entity;
}

// ----------------------------- RESOURCES ------------------------------------------------------------
Entity & AssetManager::CreateMushrooms(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Resource>(Mushrooms, scale * 300.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupResources);

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 3);
	switch (random)
	{
	case 1:
		spriteNum = "MushroomsA";
		break;
	case 2:
		spriteNum = "MushroomsB";
		break;
	case 3:
		spriteNum = "MushroomsC";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateAmberTree(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Resource>(Amber, scale * 180.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupResources);

	// randomize angle
	float random_angle = rand_float(0, 360.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 5);
	switch (random)
	{
	case 1:
		spriteNum = "AmberTreeA";
		break;
	case 2:
		spriteNum = "AmberTreeB";
		break;
	case 3:
		spriteNum = "AmberTreeC";
		break;
	case 4:
		spriteNum = "AmberTreeD";
		break;
	case 5:
		spriteNum = "AmberTreeE";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateIronDeposit(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Resource>(Iron, scale * 150.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupResources);

	// randomize angle
	float random_angle = rand_float(0, 360.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 7);
	switch (random)
	{
	case 1:
		spriteNum = "IronDepositA";
		break;
	case 2:
		spriteNum = "IronDepositB";
		break;
	case 3:
		spriteNum = "IronDepositC";
		break;
	case 4:
		spriteNum = "IronDepositD";
		break;
	case 5:
		spriteNum = "IronDepositE";
		break;
	case 6:
		spriteNum = "IronDepositF";
		break;
	case 7:
		spriteNum = "IronDepositG";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateSilicateDeposit(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Resource>(Silicate, scale * 600.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupResources);

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 2);
	switch (random)
	{
	case 1:
		spriteNum = "SilicateDepositA";
		break;
	case 2:
		spriteNum = "SilicateDepositB";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateObsidianDeposit(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Resource>(Obsidian, scale * 100.f);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupResources);

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 4);
	switch (random)
	{
	case 1:
		spriteNum = "ObsidianDepositA";
		break;
	case 2:
		spriteNum = "ObsidianDepositB";
		break;
	case 3:
		spriteNum = "ObsidianDepositC";
		break;
	case 4:
		spriteNum = "ObsidianDepositD";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

// ----------------------------- RESOURCE FIELDS ------------------------------------------------------------

void AssetManager::MakeAmberForest(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	// image, rotation already randomized

	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.06f, 0.1f);

		CreateAmberTree(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::MakeIronField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.15f, 0.27f);

		CreateIronDeposit(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::MakeSilicateField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.1f, 0.15f);

		CreateSilicateDeposit(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::MakeMushroomField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.2f, 0.25f);

		CreateMushrooms(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::MakeObsidianField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.1f, 0.15f);

		CreateObsidianDeposit(individual_position, scale, sizeX, sizeY);
	}
}

// ----------------------------- ENVIRONMENT ------------------------------------------------------------

Entity & AssetManager::CreateWetlands(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Environment>(eWetland);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupEnvironment);

	// randomize angle
	float random_angle = rand_float(-25.f, 25.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 6);
	switch (random)
	{
	case 1:
		spriteNum = "WetlandsA";
		break;
	case 2:
		spriteNum = "WetlandsB";
		break;
	case 3:
		spriteNum = "WetlandsC";
		break;
	case 4:
		spriteNum = "WetlandsD";
		break;
	case 5:
		spriteNum = "WetlandsE";
		break;
	case 6:
		spriteNum = "WetlandsF";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateThorns(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Environment>(eThorns);
	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupEnvironment);

	// randomize angle
	float random_angle = rand_float(0, 360.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 5);
	switch (random)
	{
	case 1:
		spriteNum = "ThornsA";
		break;
	case 2:
		spriteNum = "ThornsB";
		break;
	case 3:
		spriteNum = "ThornsC";
		break;
	case 4:
		spriteNum = "ThornsD";
		break;
	case 5:
		spriteNum = "ThornsE";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateScuffs(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Environment>(eScuffs);

	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupEnvironment);

	// randomize angle
	float random_angle = rand_float(0, 360.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 5);
	switch (random)
	{
	case 1:
		spriteNum = "ScuffsA";
		break;
	case 2:
		spriteNum = "ScuffsB";
		break;
	case 3:
		spriteNum = "ScuffsC";
		break;
	case 4:
		spriteNum = "ScuffsD";
		break;
	case 5:
		spriteNum = "ScuffsE";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateStones(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Environment>(eStones);

	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupEnvironment);

	// randomize angle
	int randomizer = rand_int(1, 2);
	if (randomizer == 1) transform.angle = 0.f;
	else transform.angle = 360.f;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 3);
	switch (random)
	{
	case 1:
		spriteNum = "StonesA";
		break;
	case 2:
		spriteNum = "StonesB";
		break;
	case 3:
		spriteNum = "StonesC";
		break;
	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

Entity & AssetManager::CreateFlowers(Vector2D position, float scale, int sizeX, int sizeY)
{
	Entity& entity = manager->AddEntity<Environment>(eStones);

	auto& transform = manager->addTransformComponent(entity, position.x, position.y, sizeX, sizeY, scale, true);
	manager->AddToGroup(&entity, Game::groupEnvironment);

	// randomize angle
	float random_angle = rand_float(0.f, 360.f);
	transform.angle = random_angle;

	// randomize image
	std::string spriteNum;
	int random = rand_int(1, 5);
	switch (random)
	{
	case 1:
		spriteNum = "FlowersA";
		break;
	case 2:
		spriteNum = "FlowersB";
		break;
	case 3:
		spriteNum = "FlowersC";
		break;
	case 4:
		spriteNum = "FlowersD";
		break;
	case 5:
		spriteNum = "FlowersE";
		break;

	}

	auto& sprite = manager->addSpriteComponent(entity, spriteNum, false);

	// initialize as not visible
	sprite.alpha = 0.f;

	return entity;
}

// ----------------------------- ENVIRONMENT FIELDS ------------------------------------------------------------

void AssetManager::MakeWetlandField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// randomize inidivual positions

		// Wetlands
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.17f, 0.23f);

		CreateWetlands(individual_position, scale, sizeX, sizeY);

		// Thorns
		individual_position += { rand_float(-distBetweenX/4, distBetweenY/4), rand_float(-distBetweenX/4, distBetweenY/4) };
		scale = rand_float(0.1f, 0.13f);
		CreateThorns(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::MakeScuffsField(Vector2D position, int resAmount, float distBetweenX, float distBetweenY, int sizeX, int sizeY)
{
	Vector2D individual_position = position;
	float scale;

	for (int i = 0; i < resAmount; i++)
	{
		// Scuffs
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.12f, 0.18f);

		CreateScuffs(individual_position, scale, sizeX, sizeY);
	}
	for (int i = 0; i < resAmount / 2; i++)
	{
		// Stones
		individual_position += { rand_float(-distBetweenX, distBetweenY), rand_float(-distBetweenX, distBetweenY) };
		scale = rand_float(0.07f, 0.1f);

		CreateStones(individual_position, scale, sizeX, sizeY);
	}
	for (int i = 0; i < resAmount/2; i++)
	{
		// Flowers
		individual_position += { rand_float(-distBetweenX/2, distBetweenY/2), rand_float(-distBetweenX/2, distBetweenY/2) };
		scale = rand_float(0.06f, 0.08f);

		CreateFlowers(individual_position, scale, sizeX, sizeY);
	}
}

void AssetManager::AddTexture(std::string texID, const char *path)
{
	assert(textures.find(texID) == textures.end());
	textures[texID] = TextureManager::LoadTexture(path);
}

SDL_Texture *AssetManager::GetTexture(std::string texID)
{
	return textures[texID];
}

