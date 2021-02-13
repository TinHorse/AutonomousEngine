#pragma once
#include "Components.h"
#include "Entities.h"

class Cannon : public Entity
{
public:
	Cannon(EntityManager& mManager, Entity * _owner) : Entity(mManager)
	{
		owner = dynamic_cast<Agent*>(_owner);
	}

	Cannon() = default;
	~Cannon() {}

	void update() override
	{
		bool firingState = owner->isFiring();
		this->GetComponent<AuxiliaryComponent>().setFiring(firingState);
		this->GetComponent<AuxiliaryComponent>().setTransform(owner->GetComponent<TransformComponent>());
	}

	void refresh() override
	{
		if (deletedEntities->find(owner) != deletedEntities->end())
		{
			owner = nullptr;
			this->Destroy();
		}
	}

private:
	Agent * owner;
	int angle;
	SDL_Rect srcRect;
};