#pragma once

class AISystem
{
public:
	AISystem()
	{
		
	}
	~AISystem() {}
	void Update()
	{
		/*
		index = index_state;
	for (auto& comp : compState)
	{
		const auto& tick = comp.tick;
		if (comp.type == "hunted")
		{
			if (index-- <= 0) { break; };
			comp.Update();

			if (tick % 10 == 0)
			{
				comp.state(hunger)++;
			}
			if (tick % 60 == 0 && comp.state(hunger) > 80)
			{
				comp.state(health)--;
			}

			if (comp.state(hunger) > 10)
			{
				if (tick % 60 == 0)
				{
					comp.getPathfinder()->Explore();
				}
			}

			if (tick % 60 == 0 && comp.state(hunger) > 40)
			{
				if (!comp.getPathfinder()->getTargetEntity() || comp.getPathfinder()->getTargetEntity()->GetComponent<StateComponent>().state(food) <= 0)
				{
					for (auto e : FindEntitiesInArea(comp.entity->GetComponent<TransformComponent>().position, Game::groupFood, 100))
					{
						if (e->GetComponent<StateComponent>().state(food) > 0)
						{
							comp.getPathfinder()->FindPath(e->GetComponent<TransformComponent>().position);
							break;
						}
					}
				}
			}
		}

	}
	*/
	}

private:


};

