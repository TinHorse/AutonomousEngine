#pragma once
#include "SDL.h"
#include "ConstructionManager.h"
#include "PlayerInterface.h"

class InputManager
{
public:
	void Update()
	{
		UpdateMousePosition();
	}
	
	void HandleMouseButton(SDL_MouseButtonEvent& e, EntityManager& manager)
	{
		if (Game::construction->GetIsActive())
		{
			if (e.button == SDL_BUTTON_LEFT)
			{
				if (Game::interface->HandleLeftMouseClick()) return;
				Game::construction->ConstructConstructionSite(manager, mouseX, mouseY);
				return;
			}
		}
	}

	void HandleContinuousInput()
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// Camera movement
		if (keystate[SDL_SCANCODE_W])
		{
			Game::camera.ChangeY(-0.01f);
			Game::camera.keyW = true;
		}
		if (keystate[SDL_SCANCODE_S])
		{
			Game::camera.ChangeY(0.01f);
			Game::camera.keyS = true;
		}
		if (keystate[SDL_SCANCODE_A])
		{
			Game::camera.ChangeX(-0.01f);
			Game::camera.keyA = true;
		}
		if (keystate[SDL_SCANCODE_D])
		{
			Game::camera.ChangeX(0.01f);
			Game::camera.keyD = true;
		}

	}

	void HandleKeyDown()
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		
		
		if (keystate[SDL_SCANCODE_B])
		{
			Game::construction->ToggleIsActive();
			Game::interface->bShowBuildMenu = Game::construction->GetIsActive();
			if (Game::interface->bShowMovingWidgets)
			{
				Game::interface->HideMovingWidgets();
			}
		}
	}

	void HandleKeyUp(SDL_Event& keyEvent)
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		// Camera movement
		switch (keyEvent.key.keysym.sym)
		{
		case SDLK_w:
			Game::camera.keyW = false;
		case SDLK_s:
			Game::camera.keyS = false;
		case SDLK_a:
			Game::camera.keyA = false;
		case SDLK_d:
			Game::camera.keyD = false;
		}
	}

	void HandleMouseWheel(SDL_MouseWheelEvent& e)
	{
		// UP is 1, DOWN is -1
		if (e.y == 1)
		{
			//Game::camera.ChangeGlobalScale(0.15f);
		}
		else
		{
			//Game::camera.ChangeGlobalScale(-0.15f);
		}

		/*
		if (Game::construction->GetIsActive())
		{
			// UP is 1, DOWN is -1
			if (e.y == 1)
			{
				Game::construction->ChangeAngle(10);
			}
			else
			{
				Game::construction->ChangeAngle(-10);
			}
		}
		*/
	}

	void UpdateMousePosition()
	{
		SDL_GetMouseState(&mouseX, &mouseY);
	}

	int GetMouseX()
	{
		return mouseX;
	}

	int GetMouseY()
	{
		return mouseY;
	}

	Vector2D GetMousePositionAsVector()
	{
		return Vector2D(mouseX, mouseY);
	}

private:
	int mouseX, mouseY;
	std::string currentInput;

};