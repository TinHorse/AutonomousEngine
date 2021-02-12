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

		collider_offset.edges[0] = { float(xpos), float(ypos) };
		collider_offset.edges[1] = { float(xpos) + size, float(ypos) };
		collider_offset.edges[2] = { float(xpos) + size, float(ypos + size) };
		collider_offset.edges[3] = { float(xpos), float(ypos + size) };

		dynamic = _dynamic;
	}

	void Init() override
	{
		LinkComponentPointers();
		

		texture = TextureManager::LoadTexture("assets/colliderTex.png");
		srcRect = { 0,0,320,320 };
		int x = transform->position.x;
		int y = transform->position.y;
		int w = transform->width;
		int h = transform->height;
		destRect = { x, y, w, h };

		collider_offset.edges[0] = { float(x), float(y) };
		collider_offset.edges[1] = { float(x) + w, float(y) };
		collider_offset.edges[2] = { float(x) + w, float(y + h) };
		collider_offset.edges[3] = { float(x), float(y + h) };
	}

	void Update() override
	{
		if (dynamic)
		{
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

		destRect.x = transform->position.x - Game::camera.getX();
		destRect.y = transform->position.y - Game::camera.getY();

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
								hard_collision += Collision::SAT(this->collider_offset, body->collider_offset, this->transform->centre, body->transform->centre);
							}
						}
					}
				}

				if (overridden && soft_collision.x)
				{
					//std::cout << soft_collision << std::endl;
				}

				if (hard_collision.x || hard_collision.y)
				{
					transform->speed /= 2.f;
				}

				transform->addCollisionResponse((soft_collision * 0.5f) + hard_collision);
				soft_collision.Zero();
				hard_collision.Zero();
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

