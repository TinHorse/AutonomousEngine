#pragma once
#include "Widget.h"
#include "Button.h"
#include "Player.h"
#include <string>
#include "TurretManager.h"
#include "ConfigBuildings.h"

struct PlayerInterface
{
	PlayerInterface(Player* p)
	{
		player = p;
	}

	void Init(SDL_Renderer* renderer, ConstructionManager* Construction, PlayerResourceComponent* PlayerRes, TurretManager* TurretManager, NodeSystem* nodesystem, ManufacturingManager* manufacturingManager)
	{
		construction = Construction;
		playerRes = PlayerRes;
		turretManager = TurretManager;
		manufacturing = manufacturingManager;
		node_system = nodesystem;


		// Resources
		ResourceColourWidget = new Widget(renderer, 0, 0, 2649, 340, 50, 10, 580, 60, false, true);
		ResourceColourWidget->setTexture("ResourceColours");
		widgets.push_back(ResourceColourWidget);

		MushroomWidget = new Widget(renderer, 0, 0, 200, 100, 95, 25, 55, 33, true);
		MushroomWidget->setTexture("WidgetDefault");
		widgets.push_back(MushroomWidget);

		AmberWidget = new Widget(renderer, 0, 0, 200, 100, 185, 25, 55, 33, true);
		AmberWidget->setTexture("WidgetDefault");
		widgets.push_back(AmberWidget);

		IronWidget = new Widget(renderer, 0, 0, 200, 100, 280, 25, 55, 33, true);
		IronWidget->setTexture("WidgetDefault");
		widgets.push_back(IronWidget);

		SilicateWidget = new Widget(renderer, 0, 0, 200, 100, 370, 25, 55, 33, true);
		SilicateWidget->setTexture("WidgetDefault");
		widgets.push_back(SilicateWidget);

		ObsidianWidget = new Widget(renderer, 0, 0, 200, 100, 460, 25, 55, 33, true);
		ObsidianWidget->setTexture("WidgetDefault");
		widgets.push_back(ObsidianWidget);

		MorphiteWidget = new Widget(renderer, 0, 0, 200, 100, 555, 25, 55, 33, true);
		MorphiteWidget->setTexture("WidgetDefault");
		widgets.push_back(MorphiteWidget);




		// Misc Widgets

		NumResInAreaWidget = new Widget(renderer, -10, 0, 200, 500, 0, 0, 55, 33, true, false);
		NumResInAreaWidget->setTexture("WidgetDefault");
		widgets.push_back(NumResInAreaWidget);

		NumRegularTransportersWidget = new Widget(renderer, 0, 0, 200, 100, 1400, 25, 55, 33, true);
		NumRegularTransportersWidget->setTexture("WidgetDefault");
		widgets.push_back(NumRegularTransportersWidget);

		NumConstructionTransportersWidget = new Widget(renderer, 0, 0, 200, 100, 1470, 25, 55, 33, true);
		NumConstructionTransportersWidget->setTexture("WidgetDefault");
		widgets.push_back(NumConstructionTransportersWidget);



		// Manufacture Widgets
		NumTotal_Wiringworks = new Widget(renderer, 0, 0, 200, 100, 185, 750, 55, 33, true);
		NumTotal_Wiringworks->setTexture("WidgetDefault");
		c_widgets.push_back(NumTotal_Wiringworks);

		NumTotal_Smithy = new Widget(renderer, 0, 0, 200, 100, 395, 750, 55, 33, true);
		NumTotal_Smithy->setTexture("WidgetDefault");
		c_widgets.push_back(NumTotal_Smithy);


		NumTotal_MachineFactory = new Widget(renderer, 0, 0, 200, 100, 550, 750, 55, 33, true);
		NumTotal_MachineFactory->setTexture("WidgetDefault");
		c_widgets.push_back(NumTotal_MachineFactory);


		NumTotal_Base = new Widget(renderer, 0, 0, 200, 100, 845, 750, 55, 33, true);
		NumTotal_Base->setTexture("WidgetDefault");
		c_widgets.push_back(NumTotal_Base);


		auto& texturesB = configB.BuildingSpriteNames;

		// Construction Buttons
		// INFRASTRUCTURE
		c_button_Base = new Button(renderer, 0, 0, 712, 712, 100, 800, 40, 40, Base);
		c_button_Base->setTexture(texturesB[Base]);
		c_buttons.push_back(c_button_Base);

		c_button_Gaslight = new Button(renderer, 0, 0, 400, 400, 160, 800, 40, 40, Gaslight);
		c_button_Gaslight->setTexture(texturesB[Gaslight]);
		c_buttons.push_back(c_button_Gaslight);

		c_button_Accelerator = new Button(renderer, 0, 0, 713, 713, 220, 800, 40, 40, Accelerator);
		c_button_Accelerator->setTexture(texturesB[Accelerator]);
		c_buttons.push_back(c_button_Accelerator);

		c_button_Lighthouse = new Button(renderer, 0, 0, 713, 713, 280, 800, 40, 40, Lighthouse);
		c_button_Lighthouse->setTexture(texturesB[Lighthouse]);
		c_buttons.push_back(c_button_Lighthouse);

		// RESOURCE BUILDINGS
		c_button_GatheringStation = new Button(renderer, 0, 0, 400, 400, 370, 800, 40, 40, GatheringStation);
		c_button_GatheringStation->setTexture(texturesB[GatheringStation]);
		c_buttons.push_back(c_button_GatheringStation);

		c_button_Syphon = new Button(renderer, 0, 0, 400, 400, 430, 800, 40, 40, Syphon);
		c_button_Syphon->setTexture(texturesB[Syphon]);
		c_buttons.push_back(c_button_Syphon);

		c_button_Scraper = new Button(renderer, 0, 0, 400, 400, 490, 800, 25, 40, Scraper);
		c_button_Scraper->setTexture(texturesB[Scraper]);
		c_buttons.push_back(c_button_Scraper);

		c_button_Harvester = new Button(renderer, 0, 0, 400, 400, 550, 800, 40, 40, Harvester);
		c_button_Harvester->setTexture(texturesB[Harvester]);
		c_buttons.push_back(c_button_Harvester);

		//c_button_Growhouse = new Button(renderer, 0, 0, 800, 800, 670, 800, 40, 40, Growhouse);
		//c_button_Growhouse->setTexture(texturesB[Growhouse]);
		//c_buttons.push_back(c_button_Growhouse);

		c_button_CuttingRig = new Button(renderer, 0, 0, 452, 453, 610, 800, 40, 40, CuttingRig);
		c_button_CuttingRig->setTexture(texturesB[CuttingRig]);
		c_buttons.push_back(c_button_CuttingRig);

		// MANUFACTURING
		c_button_Smithy = new Button(renderer, 0, 0, 800, 800, 700, 800, 40, 40, Smithy);
		c_button_Smithy->setTexture(texturesB[Smithy]);
		c_buttons.push_back(c_button_Smithy);

		c_button_WiringWorks = new Button(renderer, 0, 0, 713, 713, 760, 800, 40, 40, Wiringworks);
		c_button_WiringWorks->setTexture(texturesB[Wiringworks]);
		c_buttons.push_back(c_button_WiringWorks);

		c_button_MachineFactory = new Button(renderer, 0, 0, 800, 800, 820, 800, 40, 40, MachineFactory);
		c_button_MachineFactory->setTexture(texturesB[MachineFactory]);
		c_buttons.push_back(c_button_MachineFactory);

		// Hangar and Turret Manufacturing
		c_button_Hangar = new Button(renderer, 0, 0, 800, 475, 880, 800, 40, 24, Hangar);
		c_button_Hangar->setTexture(texturesB[Hangar]);
		c_buttons.push_back(c_button_Hangar);

		// Transporter Buttons
		t_button_Transporter = new Button(renderer, 0, 0, 712, 712, 1200, 800, 20, 40, NoBuilding, tTransporter);
		t_button_Transporter->setTexture("transporter_empty");
		c_buttons.push_back(t_button_Transporter);

	}
	

	void Update(SDL_Renderer* renderer, long int frameNum)
	{
		if (frameNum % 60 == 0)
		{
			UpdateStaticWidgets(renderer);
		}
		if (bShowBuildMenu)
		{
			UpdateButtonsConstruction();
			UpdateMovingWidgets(renderer);
		}
	}

	void UpdateStaticWidgets(SDL_Renderer* renderer)
	{
		// Resources
		MushroomWidget->UpdateText(renderer, std::to_string(playerRes->rMushrooms));
		AmberWidget->UpdateText(renderer, std::to_string(playerRes->rAmber));
		IronWidget->UpdateText(renderer, std::to_string(playerRes->rIron));
		SilicateWidget->UpdateText(renderer, std::to_string(playerRes->rSilicate));
		ObsidianWidget->UpdateText(renderer, std::to_string(playerRes->rObsidian));
		MorphiteWidget->UpdateText(renderer, std::to_string(playerRes->rMorphite));

		// Transporters
		NumRegularTransportersWidget->UpdateText(renderer, std::to_string(node_system->GetNumRegularTransporters()));
		//NumConstructionTransportersWidget->UpdateText(renderer, std::to_string(node_system->GetNumConstructionTransporters()));

		// Manufactures
		if (bShowBuildMenu)
		{
			NumTotal_Base->UpdateText(renderer, std::to_string(manufacturing->GetNumIdleManufactures(Base)) + "/" + std::to_string(manufacturing->GetNumTotalManufactures(Base)));
			NumTotal_Smithy->UpdateText(renderer, std::to_string(manufacturing->GetNumIdleManufactures(Smithy)) + "/" + std::to_string(manufacturing->GetNumTotalManufactures(Smithy)));
			NumTotal_Wiringworks->UpdateText(renderer, std::to_string(manufacturing->GetNumIdleManufactures(Wiringworks)) + "/" + std::to_string(manufacturing->GetNumTotalManufactures(Wiringworks)));
			NumTotal_MachineFactory->UpdateText(renderer, std::to_string(manufacturing->GetNumIdleManufactures(MachineFactory)) + "/" + std::to_string(manufacturing->GetNumTotalManufactures(MachineFactory)));
		}
	}

	void UpdateMovingWidgets(SDL_Renderer* renderer)
	{
		Building* building = construction->GetBuilding();
		if (construction->ShouldSumResourcesInArea())
		{
			bShowMovingWidgets = true;
			NumResInAreaWidget->UpdateText(renderer, std::to_string(construction->amountResInArea));
			TransformComponent& buildingTransform = building->GetComponent<TransformComponent>();
			NumResInAreaWidget->textRect.x = buildingTransform.position.x + buildingTransform.width / 2 * buildingTransform.scale;
			NumResInAreaWidget->textRect.y = buildingTransform.position.y + buildingTransform.height / 2 * buildingTransform.scale;
			return;
		}
		HideMovingWidgets();
	}

	void HideMovingWidgets()
	{
		if (bShowMovingWidgets)
		{
			bShowMovingWidgets = false;
			NumResInAreaWidget->textRect.x = 0;
			NumResInAreaWidget->textRect.y = 0;
		}
	}

	void UpdateButtonsConstruction()
	{
		for (Button* b : c_buttons)
		{
			b->UpdateCollision(MousePosition.x, MousePosition.y);
			
			if (b->buildingType != NoBuilding)
			{
				// check for sufficient resources
				if (construction->GetHasSufficientResourcesConstruction(b->buildingType, playerRes))
				{
					b->bHasEnoughResources = true;
					continue;
				}
				b->bHasEnoughResources = false;
				continue;
			}

			
			if (b->turretType != tNone)
			{
				if (manufacturing->GetHasSufficientResourcesManufacturing(NoBuilding, b->turretType, playerRes))
				{
					if (manufacturing->CanManufactureTurret(b->turretType))
					{
						b->bHasEnoughResources = true;
						continue;
					}
				}
				b->bHasEnoughResources = false;
				continue;
			}
			
		}
	}

	bool HandleLeftMouseClick()
	{
		bool handled = false;
		if (bShowBuildMenu)
		{
			for (Button* b : c_buttons)
			{
				if (b->bIsColliding)
				{
					if (b->buildingType != NoBuilding)
					{
						if (b->bHasEnoughResources)
						{
							construction->SetBuildingType(b->buildingType);
							construction->SetBuildingWasSwitched(true);
							construction->SetIsBuildingSelected(true);
						}
						handled = true;
						continue;
					}
					if (b->turretType != tNone)
					{
						if (b->bHasEnoughResources)
						{
							manufacturing->AddTurretManufacturingRequest(b->turretType);
						}
						handled = true;
						continue;
					}
				}
			}
		}
		return handled;
	}

	void DrawWidgets()
	{
		for (Widget* w : widgets)
		{
			w->Draw();
		}
		if (bShowBuildMenu)
		{
			for (Button* b : c_buttons)
			{
				b->Draw();
			}
			for (Widget* w : c_widgets)
			{
				w->Draw();
			}
		}
	}

	void UpdateMousePosition(float x, float y)
	{
		MousePosition.x = x;
		MousePosition.y = y;
	}

	Player* player = nullptr;

	std::vector<Widget*> widgets;
	std::vector<Widget*> c_widgets;
	std::vector<Button*> c_buttons;

	Vector2D MousePosition;

	ConstructionManager* construction = nullptr;
	PlayerResourceComponent* playerRes = nullptr;
	TurretManager* turretManager = nullptr;
	ManufacturingManager* manufacturing = nullptr;
	NodeSystem* node_system = nullptr;

	bool bShowBuildMenu = false;
	int numResInArea = 0;
	bool bShowMovingWidgets = false;

	// CONFIG
	ConfigBuildings configB;


	// WIDGETS -------------------------------
	// Resource Widgets
	Widget* ResourceColourWidget;

	Widget* MushroomWidget;
	Widget* AmberWidget;
	Widget* IronWidget;
	Widget* SilicateWidget;
	Widget* ObsidianWidget;
	Widget* MorphiteWidget;

	// Misc Widgets
	Widget* NumResInAreaWidget;

	Widget* NumRegularTransportersWidget;
	Widget* NumConstructionTransportersWidget;

	// Manufacture Widgets
	Widget* NumTotal_Base;
	Widget* NumTotal_Smithy;
	Widget* NumTotal_Wiringworks;
	Widget* NumTotal_MachineFactory;


	// BUTTONS -------------------------------
	// Construction Buttons 
	Button* c_button_Base;
	Button* c_button_Gaslight;
	Button* c_button_Syphon;
	Button* c_button_Scraper;
	Button* c_button_Harvester;
	Button* c_button_GatheringStation;
	Button* c_button_MachineFactory;
	Button* c_button_WiringWorks;
	Button* c_button_Accelerator;
	Button* c_button_Lighthouse;
	Button* c_button_Growhouse;
	Button* c_button_Smithy;
	Button* c_button_Hangar;
	Button* c_button_CuttingRig;

	// Turret Buttons
	Button* t_button_Transporter;
};