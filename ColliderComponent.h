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
	Rect collider_offset;

	std::string tag;

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
	}
	ColliderComponent(std::string mTag, int xpos, int ypos, int size, bool _dynamic)
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
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
		destRect = { collider.x, collider.y, collider.w, collider.h };

		collider_offset.edges[0] = { float(collider.x), float(collider.y) };
		collider_offset.edges[1] = { float(collider.x) + collider.w, float(collider.y) };
		collider_offset.edges[2] = { float(collider.x) + collider.w, float(collider.y + collider.h) };
		collider_offset.edges[3] = { float(collider.x), float(collider.y + collider.h) };
	}

	void Update() override
	{
		if (dynamic)
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);

			double rad_angle = toRad(transform->angle);
			auto pos = transform->position;
			Vector2D centre;
			float sc = transform->scale;

			centre.x = transform->centre.x;
			centre.y = transform->centre.y;

			collider_offset.edges[0] = rotate_point(0,0, rad_angle, Vector2D(- sc * transform->width /2, -sc * transform->height/2)).Add(centre);
			collider_offset.edges[1] = rotate_point(0,0, rad_angle, Vector2D(sc * transform->width / 2, -sc * transform->height / 2)).Add(centre);
			collider_offset.edges[2] = rotate_point(0,0, rad_angle, Vector2D(sc * transform->width / 2, sc * transform->height / 2)).Add(centre);
			collider_offset.edges[3] = rotate_point(0,0, rad_angle, Vector2D(-sc * transform->width / 2, sc * transform->height / 2)).Add(centre);
			

			//collider_offset.edges[0] = { float(collider.x), float(collider.y) };
			//collider_offset.edges[1] = { float(collider.x) + collider.w, float(collider.y) };
			//collider_offset.edges[2] = { float(collider.x) + collider.w, float(collider.y + collider.h) };
			//collider_offset.edges[3] = { float(collider.x), float(collider.y + collider.h) };
		}

		destRect.x = collider.x - Game::camera.getX();
		destRect.y = collider.y - Game::camera.getY();

		collision.registerCollider(this);
		if (dynamic)
		{
			if (current_cell)
			{
				for (auto* c : current_cell->getRegion())
				{
					for (auto* body : c->colliders)
					{
						if (body != this)
						{
							if (body->dynamic)
							{
								soft_collision += Collision::SAT(this->collider_offset, body->collider_offset, this->transform->centre, body->transform->centre);
							}
							else
							{
								soft_collision += Collision::SAT(this->collider_offset, body->collider_offset, this->transform->centre, body->transform->centre) * 2.f;
							}
						}
					}
				}

				if (overridden && soft_collision.x)
				{
					//std::cout << soft_collision << std::endl;
				}

				transform->addCollisionResponse(soft_collision);
				soft_collision.Zero();
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

