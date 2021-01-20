#pragma once
#include <string>
#include "SDL.h"
#include "TextureManager.h"
#include "Collision.h"
#include "CollisionMesh.h"
#include "Components.h"

extern Collisionmesh collision;

// The collider component is used to define a collision rectangle that checks for collisions with other rectangles

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	ColCell* current_cell = nullptr;
	
	Vector2D force;
	bool dynamic = false;
	bool isColliding = false;

	TransformComponent *transform;
	ColliderComponent() = default;
	ColliderComponent(std::string mTag, bool _dynamic)
	{
		tag = mTag;
		dynamic = _dynamic;
	}
	ColliderComponent(std::string mTag, int xpos,  int ypos, int size, bool _dynamic)
	{
		tag = mTag;
		collider.x = xpos;
		collider.y = ypos;
		collider.w = collider.h = size;
		dynamic = _dynamic;
	}

	void Init() override
	{
		LinkComponentPointers();
		

		texture = TextureManager::LoadTexture("assets/colliderTex.png");
		srcRect = { 0,0,320,320 };
		collider.x = transform->position.x;
		collider.y = transform->position.y;
		collider.w = transform->width;
		collider.h = transform->height;
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void Update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destRect.x = collider.x - Game::camera.getX();
		destRect.y = collider.y - Game::camera.getY();

		collision.registerCollider(this);
		if (dynamic)
		{
			isColliding = false;
			force.Zero();
			if (current_cell)
			{
				for (auto c : current_cell->getRegion())
				{
					for (auto body : c->colliders)
					{
						if (body != this)
						{
							if (Collision::AABB(*this, *body))
							{
								force += Collision::CalculateOpposingForce(*this, *body);
							}
						}
					}
				}
			}
			transform->addCollisionResponse(force);
			if (force.x || force.y)
			{
				isColliding = true;
			}
		}

	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void LinkComponentPointers() override
	{
		transform = &entity->GetComponent<TransformComponent>();
	}

};

