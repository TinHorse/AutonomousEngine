#pragma once
#include "SDL.h"
#include "Vector2D.h"

class Node;

class Connection
{
public:
	Connection() = default;

	Connection(float x, float y, float x2, float y2)
	{
		posX = x;
		posY = y;
		posX2 = x2;
		posY2 = y2;
	}

	void Update(Vector2D camera_offset)
	{
		posX -= camera_offset.x;
		posX2 -= camera_offset.x;
		posY -= camera_offset.y;
		posY2 -= camera_offset.y;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
		if (nodes[0] && nodes[1])
			SDL_RenderDrawLine(renderer, posX, posY, posX2, posY2);
		else
			Destroy();
	}

	void Destroy()
	{
		isActive = false;
	}

	bool getIsActive()
	{
		return isActive;
	}

	void SetNodes(Node* node1, Node* node2)
	{
		nodes[0] = node1;
		nodes[1] = node2;
	}

	void SetPosition(Vector2D pos1, Vector2D pos2)
	{
		posX = pos1.x;
		posY = pos1.y;
		posX2 = pos2.x;
		posY2 = pos2.y;
	}

	int alpha = 50;

private:
	bool isActive = true;
	float posX, posY, posX2, posY2;
	std::array<Node*, 2> nodes;

	
};

