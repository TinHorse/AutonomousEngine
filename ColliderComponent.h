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
	Vector2D centre;

	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	ColCell* current_cell = nullptr;
	
	Vector2D soft_collision;
	Vector2D hard_collision;
	bool dynamic = false;
	bool overridden = false;

	TransformComponent *transform;
	ColliderComponent() = default;
	ColliderComponent(std::string mTag, bool _dynamic, bool _overridden = false)
	{
		tag = mTag;
		dynamic = _dynamic;
		overridden = _overridden;

		centre.x = this->collider.x + (this->collider.w / 2);
		centre.y = this->collider.y + (this->collider.h / 2);
	}
	ColliderComponent(std::string mTag, int xpos, int ypos, int size, bool _dynamic)
	{
		tag = mTag;
		collider.x = xpos;
		collider.y = ypos;
		collider.w = collider.h = size;
		dynamic = _dynamic;

		centre.x = this->collider.x + (this->collider.w / 2);
		centre.y = this->collider.y + (this->collider.h / 2);
	}

	void Init() override
	{
		LinkComponentPointers();
		

		texture = TextureManager::LoadTexture("assets/colliderTex.png");
		srcRect = { 0,0,320,320 };
		collider.x = transform->position.x;
		collider.y = transform->position.y;
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void Update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			//collider.w = transform->width * transform->scale;
			//collider.h = transform->height * transform->scale;
		}

		destRect.x = collider.x - Game::camera.getX();
		destRect.y = collider.y - Game::camera.getY();

		collision.registerCollider(this);
		if (dynamic)
		{
			if (current_cell)
			{
				centre.x = this->collider.x + (this->collider.w / 2);
				centre.y = this->collider.y + (this->collider.h / 2);
				for (auto* c : current_cell->getRegion())
				{
					for (auto* body : c->colliders)
					{
						if (body != this)
						{
							if (Collision::AABB(*this, *body))
							{
								if (body->dynamic)
								{
									soft_collision += Collision::CalculateOpposingForce(*this, *body, centre, body->centre);
									body->addSoftCollisionOpposite(soft_collision);
								}
								else
								{
									hard_collision += Collision::CalculateOpposingForce(*this, *body, centre, body->centre);
								}
							}
						}
					}
				}
				if (hard_collision.x || hard_collision.y || overridden)
				{
					soft_collision.Zero();
				}

				transform->addCollisionResponse(soft_collision + hard_collision);
				soft_collision.Zero();
				hard_collision.Zero();
			}
		}
		else
		{
			for (auto* c : current_cell->getRegion())
			{
				for (auto* body : c->colliders)
				{
					if (body != this)
					{
						if (Collision::AABB(*this, *body))
						{
							body->addHardCollisionOpposite(Collision::CalculateOpposingForce(*this, *body, centre, body->centre));
						}
					}
				}
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

	void addSoftCollisionOpposite(const Vector2D& f)
	{
		soft_collision -= f;
	}

	void addHardCollisionOpposite(const Vector2D& f)
	{
		hard_collision -= f;
	}

};

