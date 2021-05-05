#pragma once
#include "NodeSystem.h"
#include <map>
#include "Player.h"
#include "TurretManager.h"
#include "ManufacturingManager.h"
#include "ConfigCosts.h"

struct ShadowBuilding
{
	ShadowBuilding() {}
	~ShadowBuilding() {}

	Entity* entity = nullptr;
	Node shadow_node;
	bool bNode1 = false;
	bool bNode2 = false;
	Node* facingNode = nullptr;

	int x{ 0 }, x1{ 0 }, x2{ 0 }, y{ 0 }, y1{ 0 }, y2{ 0 };

	void Update()
	{
		if (!entity) return;
		bNode1 = bNode2 = false;
		TransformComponent& transform = entity->GetComponent<TransformComponent>();
		x = shadow_node.posX = transform.position.x;
		y = shadow_node.posY = transform.position.y;

		bool bIsManufacture = entity->HasComponent<ManufacturingComponent>();

		std::vector<Node*> foundNodes = Game::node_system->FindNodesInArea(&shadow_node, transform.position.x, transform.position.y, bIsManufacture);
		for (Node* node : foundNodes)
		{
			if (node)
			{
				if (!bNode1)
				{
					facingNode = node;
					bNode1 = true;
					x1 = node->GetPosition().x;
					y1 = node->GetPosition().y;
				}
				else
				{
					bNode2 = true;
					x2 = node->GetPosition().x;
					y2 = node->GetPosition().y;
				}
			}
		}		
		if (!bNode1)
			facingNode = nullptr;
	}

	void Draw(SDL_Renderer* renderer)
	{
		// Draw connections
		if(bNode1)
			SDL_RenderDrawLine(renderer, x, y, x1, y1);
		if(bNode2)
			SDL_RenderDrawLine(renderer, x, y, x2, y2);
	}

	void Destroy()
	{
		if (entity)
		{
			entity->Destroy();
		}
		entity = nullptr;
	}
};

class ConstructionManager
{
public:
	ConstructionManager()
	{

	}

	void PartitionVectors(int Frame)
	{
		const int Frames = 30;
		Frame = Frame % Frames;

		int EntitiesPerFrame = (activeConstructions.size() / Frames) + 1;
		currentStart = Frame * EntitiesPerFrame;
		currentEnd = currentStart + EntitiesPerFrame;
	}


	void Update()
	{
		if (bIsActive)
		{
			if (bIsBuildingSelected)
			{
				UpdateShadow();

				if (!Shadow)
				{
					Shadow = new ShadowBuilding();
					Vector2D position = { MousePosition.x,MousePosition.y };

					switch (buildingType)
					{
					case NoBuilding:
						return;
					case Base:
						Shadow->entity = &Game::assets->CreateBase(position);
						break;
					case Syphon:
						Shadow->entity = &Game::assets->CreateSyphon(position);
						break;
					case Gaslight:
						Shadow->entity = &Game::assets->CreateGaslight(position);
						break;
					case Scraper:
						Shadow->entity = &Game::assets->CreateScraper(position);
						break;
					case Harvester:
						Shadow->entity = &Game::assets->CreateHarvester(position);
						break;
					case GatheringStation:
						Shadow->entity = &Game::assets->CreateGatheringStation(position);
						break;
					case MachineFactory:
						Shadow->entity = &Game::assets->CreateMachineFactory(position);
						break;
					case Wiringworks:
						Shadow->entity = &Game::assets->CreateWiringWorks(position);
						break;
					case Accelerator:
						Shadow->entity = &Game::assets->CreateAccelerator(position);
						break;
					case Lighthouse:
						Shadow->entity = &Game::assets->CreateLighthouse(position);
						break;
					case Growhouse:
						Shadow->entity = &Game::assets->CreateGrowhouse(position);
						break;
					case Smithy:
						Shadow->entity = &Game::assets->CreateSmithy(position);
						break;
					case Hangar:
						Shadow->entity = &Game::assets->CreateHangar(position);
						break;
					case CuttingRig:
						Shadow->entity = &Game::assets->CreateCuttingRig(position);
						break;
					default:
						throw;
					}
				}

				const bool bNoResources = !GetHasSufficientResourcesConstruction(buildingType, &Game::player->GetComponent<PlayerResourceComponent>());
				const bool bHasBuildingTypeStored = Game::player->GetComponent<PlayerResourceComponent>().buildings[buildingType] > 0;

				if (bBuildingSwitched || (bNoResources && bHasBuildingTypeStored))
				{
					if (Shadow)
						DestroyShadow();
					bBuildingSwitched = false;
					return;
				}

				Vector2D position = { MousePosition.x, MousePosition.y };

				Shadow->entity->GetComponent<TransformComponent>().position = Vector2D(position);
				if (Shadow->facingNode)
				{
					Shadow->entity->GetComponent<TransformComponent>().SetViewTarget(Shadow->facingNode->GetPosition());
				}
				return;
			}
			if (Shadow)
			{
				DestroyShadow();
				return;
			}
		}
		if (Shadow)
		{
			SetIsBuildingSelected(false);
			DestroyShadow();
			return;
		}
	}



	void UpdateActiveConstructions(EntityManager& manager, int Time)
	{
		PartitionVectors(Time);
		std::vector<ConstructionSite*> toBeDestroyed;
		for (int i = currentStart; i < currentEnd; i++)
		{
			if (i >= activeConstructions.size()) return;
			ConstructionSite* construction = activeConstructions[i];
			SendRequests(construction);
			if(construction->bIsFinished)
			{
				TransformComponent& transform = construction->GetComponent<TransformComponent>();
				ConstructBuilding(manager, construction->GetBuildingType(), transform.position, construction->GetNode(), transform.angle);
				construction->MarkedForDestruction = true;
				toBeDestroyed.push_back(construction);

			}
		}

		// destroy finished construction sites
		activeConstructions.erase(std::remove_if(activeConstructions.begin(), activeConstructions.end(), // iterates through
			[](ConstructionSite* site) // creates unique ptr to each element
		{
			return site->MarkedForDestruction; // for each element, checks if active
		}),
			activeConstructions.end());

		// Destroy constructions
		for (ConstructionSite* site : toBeDestroyed)
		{
			site->Destroy();
		}
	}

	void SendRequests(ConstructionSite* construction)
	{
		if (!construction->ShouldRequestResources()) return;

		ResourceHeap& resources_needed = construction->resources_needed;
		int& NumActiveDeliveries = construction->NumActiveDeliveries;
		Node* node = construction->GetNode();

		if (resources_needed.rAmber > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Amber, node);
			resources_needed.rAmber--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rIron > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Iron, node);
			resources_needed.rIron--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rMorphite > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Morphite, node);
			resources_needed.rMorphite--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rMushrooms > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Mushrooms, node);
			resources_needed.rMushrooms--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rObsidian > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Obsidian, node);
			resources_needed.rObsidian--; NumActiveDeliveries++; return;
		}
		if (resources_needed.rSilicate > 0)
		{
			Game::node_system->AddRequestAtFront(Delivery, Silicate, node);
			resources_needed.rSilicate--; NumActiveDeliveries++; return;
		}
	}

	void SumResourcesInArea(EntityManager& manager)
	{
		Building* building = GetBuilding();
		if (building)
		{
			Vector2D position = building->GetComponent<TransformComponent>().position;
			float dist = building->range;
			
			ResourceType type = building->GetComponent<ProductionComponent>().GetResourceType();
			const std::vector<Resource*> resources = manager.FindEntitiesInAreaByType<Resource>(position, Game::groupResources, dist);

			amountResInArea = 0;
			for (Resource* res : resources)
			{
				if (res->ResourceType == type)
				{
					amountResInArea += res->resourceAmount;
				}
			}
		}
	}

	std::vector<Resource*> GetResourcesInArea(EntityManager& manager, Vector2D position, float range)
	{
		std::vector<Resource*> res;
		res = manager.FindEntitiesInAreaByType<Resource>(position, Game::groupResources, range);
		return res;
	}


	bool ShouldSumResourcesInArea()
	{
		if (!bIsBuildingSelected) return false;
		if (!Shadow) return false;
		if (!Shadow->entity) return false;
		if (!Shadow->entity->HasComponent<ProductionComponent>()) return false;
		return true;
	}

	Building* GetBuilding()
	{
		if (!Shadow) return nullptr;
		Building* building = dynamic_cast<Building*>(Shadow->entity);
		if (!building) return nullptr;
		return building;
	}

	void DestroyShadow()
	{
		Shadow->Destroy();
		delete Shadow;
		Shadow = nullptr;
	}

	void ChangeAngle(float a)
	{
		angle += a;
	}

	void UpdateMousePosition(float x, float y)
	{
		MousePosition.x = x;
		MousePosition.y = y;
	}

	void ConstructConstructionSite(EntityManager& manager, int x, int y)
	{
		if (!Shadow) return;
		if (buildingType == NoBuilding) return;

		NodeSystem* nodeSystem = Game::node_system;
		ResourceHeap NeededResources = Costs.BuildingCosts[buildingType];

		Entity& entity = Game::assets->CreateConstructionSite(buildingType, Vector2D(x, y));
		if (Shadow->facingNode)
		{
			entity.GetComponent<TransformComponent>().SetViewTarget(Shadow->facingNode->GetPosition());
		}

		ConstructionSite* Construction = dynamic_cast<ConstructionSite*>(&entity);
		if (!Construction) return;

		activeConstructions.push_back(Construction);

		// add node for construction site
		Node* node = nodeSystem->AddNode(x, y, Construction, false);

		// check if first building
		if (firstBase)
		{
			firstBase = false;
			Game::node_system->InitAtFirstStorage(Vector2D(x, y));
			Construction->bIsFinished = true;
			bBuildingSwitched = true;
			DestroyShadow();
			return;
		}

		// Request manufacture of building
		if (Game::player->GetComponent<PlayerResourceComponent>().buildings[buildingType] < 1)
		{
			Game::manufacturingManager->AddBuildingManufacturingRequest(buildingType);
		}
	}


	void ConstructBuilding(EntityManager& manager, BuildingType type, Vector2D position, Node* node, float Angle)
	{
		Entity* building = nullptr;
		bool bIsGlobalStorage = false;
		bool bIsAccelerator = false;

		switch (type)
		{
		case NoBuilding:
			return;
		case Base:
			building = &Game::assets->CreateBase(position);
			bIsGlobalStorage = true;
			updateLightingIntensity = 125.f;
			break;
		case Gaslight:
			building = &Game::assets->CreateGaslight(position);
			updateLightingIntensity = 75.f;
			break;
		case Syphon:
			building = &Game::assets->CreateSyphon(position);
			break;
		case Scraper:
			building = &Game::assets->CreateScraper(position);
			break;
		case Harvester:
			building = &Game::assets->CreateHarvester(position);
			break;
		case GatheringStation:
			building = &Game::assets->CreateGatheringStation(position);
			break;
		case MachineFactory:
			building = &Game::assets->CreateMachineFactory(position);
			break;
		case Wiringworks:
			building = &Game::assets->CreateWiringWorks(position);
			break;
		case Accelerator:
			building = &Game::assets->CreateAccelerator(position);
			bIsAccelerator = true;
			break;
		case Lighthouse:
			building = &Game::assets->CreateLighthouse(position);
			updateLightingIntensity = 150.f;
			break;
		case Growhouse:
			building = &Game::assets->CreateGrowhouse(position);
			break;
		case Smithy:
			building = &Game::assets->CreateSmithy(position);
			break;
		case Hangar:
			building = &Game::assets->CreateHangar(position);
			break;
		case CuttingRig:
			building = &Game::assets->CreateCuttingRig(position);
			break;
		default:
			throw;
		}

		if (updateLightingIntensity < 10.f)
		{
			updateLightingIntensity = 10.f;
		}

		if (building)
		{
			LastBuilding = building;
			building->GetComponent<TransformComponent>().angle = Angle;

			// add building to node
			node->SetEntity(building);

			if (bIsGlobalStorage)
			{
				node->bIsGlobalResourceStorage = true;
			}
			if (bIsAccelerator)
			{
				node->bIsAccelerator = true;
			}
			
			// add node to building
			Building* b = dynamic_cast<Building*>(building);
			b->SetNode(node);

			// add manufacture to ManufacturingManager
			if (b->HasComponent<ManufacturingComponent>())
			{
				Game::manufacturingManager->AddManufacture(b, type);
			}

			// Add resources in area if has production component
			if (building->HasComponent<ProductionComponent>())
			{
				Vector2D position = node->GetPosition();
				float range = b->range;

				ResourceType resType = building->GetComponent<ProductionComponent>().GetResourceType();
				for (Resource* res : GetResourcesInArea(manager, position, range))
				{
					if (res->ResourceType == resType)
					{
						building->GetComponent<ProductionComponent>().AddResource(res);
						res->AddCurrentlyHarvestingComponent(&building->GetComponent<ProductionComponent>());
					}
				}
			}
		}
	}

	void DeductResourcesFromPlayer(const ResourceHeap& cost, const BuildingType& type, PlayerResourceComponent& playerRes)
	{
		playerRes.rMushrooms -= cost.rMushrooms;
		playerRes.rAmber -= cost.rAmber;
		playerRes.rIron -= cost.rIron;
		playerRes.rSilicate -= cost.rSilicate;
		playerRes.rObsidian -= cost.rObsidian;
		playerRes.rMorphite -= cost.rMorphite;
		playerRes.buildings[type]--;
	}

	bool GetHasSufficientResourcesConstruction(BuildingType buildingType, PlayerResourceComponent* playerResources)
	{
		ResourceHeap& costs = Costs.BuildingCosts[buildingType];

		if (playerResources->rAmber >= costs.rAmber
			&& playerResources->rIron >= costs.rIron
			&& playerResources->rMorphite >= costs.rMorphite
			&& playerResources->rMushrooms >= costs.rMushrooms
			&& playerResources->rObsidian >= costs.rObsidian
			&& playerResources->rSilicate >= costs.rSilicate)
		{
			return true;
		}
		return false;
	}

	void SetIsBuildingSelected(bool s)
	{
		bIsBuildingSelected = s;
	}

	bool GetIsBuildingSelected()
	{
		return bIsBuildingSelected;
	}

	void ToggleIsActive()
	{
		bIsActive = bIsActive ? false : true;
		if (!bIsActive)
		{
			buildingType = NoBuilding;
		}
	}

	void SetIsActive(bool active)
	{
		bIsActive = active;
		if (!active)
		{
			buildingType = NoBuilding;
		}
	}

	bool GetIsActive()
	{
		return bIsActive;
	}

	void SetBuildingWasSwitched(bool s)
	{
		bBuildingSwitched = s;
	}

	BuildingType GetBuildingType()
	{
		return buildingType;
	}

	void SetBuildingType(BuildingType type)
	{
		buildingType = type;
	}

	void UpdateShadow()
	{
		if (Shadow)
		{
			Shadow->Update();
		}
	}

	void DrawShadow(SDL_Renderer* renderer)
	{
		if (Shadow)
		{
			Shadow->Draw(renderer);
		}
	}

	float updateLightingIntensity = 0.f;
	Entity* LastBuilding = nullptr;

	int amountResInArea = 0;

private:
	bool bIsActive = false;
	bool bIsBuildingSelected = false; // change to false per default
	bool bBuildingSwitched = false;
	ShadowBuilding* Shadow = nullptr;

	BuildingType buildingType = NoBuilding;

	Vector2D MousePosition;
	float angle = 0.f;

	// query active constructions to build them
	std::vector<ConstructionSite*> activeConstructions;

	// make exception for very first base
	bool firstBase = true;


	// Partitioning
	int currentStart = 0;
	int currentEnd = 0;

	EntityCosts Costs;
	ConfigBuildings configB;
};