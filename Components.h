#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "KeyboardController.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "PathfindingComponent.h"
#include "AIComponent.h"

enum ComponentType : std::size_t
{
	Transform,
	Sprite,
	Keyboard,
	Collider,
	Tile,
	Pathfinding,
	AI
};