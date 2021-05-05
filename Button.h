#pragma once
#include "Widget.h"

struct Button : Widget
{
	Button(SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int x, int y, int w, int h, BuildingType building_type = NoBuilding, TurretType turret_type = tNone) : Widget(renderer, srcX, srcY, srcW, srcH, x, y, w, h, false)
	{
		buildingType = building_type;
		turretType = turret_type;
	}

	bool GetIsColliding(int mouseX, int mouseY)
	{
		if(mouseX > destRect.x && mouseX < (destRect.x + destRect.w)
			&& mouseY > destRect.y && mouseY < (destRect.y + destRect.h))
		{
			return true;
		}
		return false;
	}

	void UpdateCollision(int mouseX, int mouseY) override
	{
		bIsColliding = GetIsColliding(mouseX, mouseY);

		if (bHasEnoughResources && bIsColliding)
		{
			SetAlpha(255);
			return;
		}
		if (bHasEnoughResources)
		{
			SetAlpha(150);
			return;
		}
		SetAlpha(75);
	}


	bool bIsColliding = false;
	bool bHasEnoughResources = false;
	bool bIsSelectable = false;

	BuildingType buildingType;
	TurretType turretType;
};