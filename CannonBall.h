#pragma once
#include "Components.h"

class CannonBall : public Agent
{
public:
	CannonBall(EntityManager& mManager) : Agent(mManager) {}

	CannonBall() = default;
	~CannonBall() {}

	void update() override
	{
		lifetime--;
		if (lifetime < 0)
		{
			this->Destroy();
			return;
		}

		if (GetComponent<ColliderComponent>().isColliding)
		{
			auto& targets = manager.FindEntitiesInArea(GetComponent<TransformComponent>().position, Game::groupHunted, 100);

			for (auto* t : targets)
			{
				if (Collision::SATQuery(this->GetComponent<ColliderComponent>().collider_offset, t->GetComponent<ColliderComponent>().collider_offset, 0.5f))
				{

					auto* temp = dynamic_cast<Hunted*>(t);
					if (temp)
					{
						temp->getHealth() -= 100;
						if (temp->getHealth() <= 0) temp->whenDead();
						this->Destroy();

						return;
					}
				}
			}
		}

	}
	
	void updateState() override {};
	void updateQueries() override {};
	void updateBehaviour() override {};
	void whenDead() override {};

private:
	int lifetime = 60;
};