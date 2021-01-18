

class AISystem
{
public:
	AISystem() {}
	~AISystem() {}


	/*
	void Update()
	{
		for (auto& entity : manager.GetGroup(Game::groupHunted))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				updateHunted(*entity);
			}
			if (ticks % 30 == 0)
			{
				performBehaviourHunted(entity, ticks);
			}
		}

		for (auto& entity : manager.GetGroup(Game::groupPredators))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				updatePredators(*entity);
			}
			if (ticks % 30 == 0)
			{
				performBehaviourPredators(entity, ticks);
			}
		}

		for (auto& entity : manager.GetGroup(Game::groupFood))
		{
			auto ticks = entity->incrementTicks();
			if (ticks % 60 == 0)
			{
				updateFoodSources(*entity);
			}
		}
	}

	void performBehaviourHunted(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();

		entity->GetComponent<SpriteComponent>().PlayAnim();

		//execute state
		Result result;
		switch (state.currentBehaviour())
		{
		case idle:
			break;

		case exploring:
			if (s_exploring(entity, Game::groupFood, 100))
			{
				state.popBehaviour();
				state.pushBehaviour(eating, 0);
				state.pushBehaviour(followTarget, 0);
			}
			break;

		case followTarget:
			result = s_followTarget(entity, state.getTarget("current"), 20);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rNO_TARGET:
				state.popBehaviour();
				break;
			default:
				break;
			}
			break;

		case eating:
			std::cout << "eating" << std::endl;
			result = s_transferState(entity, state.getTarget("current"), "food", 1, -1, 10, 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				state.setTarget("current", nullptr);
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 0);
				break;
			case rNO_TARGET:
				state.popBehaviour();
			default:
				break;
			}
			break;
		case goToOrigin:
			state.popBehaviour();
			state.pushBehaviour(waitingForCalm, 1);
			state.pushBehaviour(followTarget, 1);
			pathfindingQueue.makePathfindingRequest(entity, state.getTarget("origin"));
			break;

		case waitingForCalm:
			result = s_transferState(entity, state.getTarget("origin"), "calm", 100, 0, 50, 30);
			switch (result)
			{
			case rSUCCESS:
				state.setTarget("current", nullptr);
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 1);
				break;
			case rCONTINUE:
				break;
			}
			break;

		case fleeing:
			result = s_runAway(entity, state.getTarget("enemy"), 200);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				state.setTarget("enemy", nullptr);
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 2);
				break;
			case rCONTINUE:
				break;
			}
			break;

		default:
			assert(true && "FSM trying to access Behaviour function that doesnt exist");
		}
	}

	void performBehaviourPredators(Entity* entity, const long& ticks)
	{
		auto& state = entity->GetComponent<StateComponent>();

		//execute state
		Result result;
		switch (state.currentBehaviour())
		{
		case idle:
			break;

		case exploring:
			if (s_exploring(entity, Game::groupHunted, 200))
			{
				state.popBehaviour();
				state.pushBehaviour(eating, 1);
				state.pushBehaviour(attacking, 1);
				state.pushBehaviour(followTarget, 1);
				state.getTarget("current")->GetComponent<StateComponent>().setTarget("enemy", entity);
			}
			break;

		case followTarget:
			result = s_followTarget(entity, state.getTarget("current"), 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rNO_TARGET:
				state.popBehaviour();
				break;
			default:
				break;
			}
			break;

		case attacking:
			result = s_transferState(entity, state.getTarget("current"), "health", 0, -10, 101, 10);
			switch (result)
			{
			case rSUCCESS:
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 1);
				break;
			case rNO_TARGET:
				state.popBehaviour();
			default:
				break;
			}
			break;
		case eating:
			result = s_transferState(entity, state.getTarget("current"), "carrion", 10, -10, 100, 10);
			switch (result)
			{
			case rSUCCESS:
				state.setTarget("current", nullptr);
				state.popBehaviour();
				break;
			case rFAIL:
				state.pushBehaviour(followTarget, 1);
				break;
			case rNO_TARGET:
				state.popBehaviour();
			default:
				break;
			}
			break;
		default:
			assert(true && "FSM trying to access Behaviour function that doesnt exist");
		}
	}
	*/

	void update()
	{
		/*
		int index = index_hunted;
		for (auto& h : hunted)
		{
			if (index-- <= 0) { break; }
			h.update();
		}
		*/
	}

	/*
	template<typename ...TArgs>
	void addAIEntity(Entity * entity, Game::groupLabels mGroup, TArgs && ...args)
	{
		switch (mGroup)
		{
		case Game::groupHunted:
			hunted[index_hunted] = Hunted(entity, std::forward<TArgs> (args)...);
			index_hunted++;
		}
	}
	*/

	void refresh()
	{
		/*
		int index = 0;
		for (auto& h : hunted)
		{
			if (index >= index_hunted) { break; };
			if (!h)
			{
				while (index_hunted > 0 && !hunted[index_hunted - 1].entity)
				{
					index_hunted--;
				}
				if (index_hunted > 0 && index_hunted != index)
				{
					std::swap(hunted[index], hunted[index_hunted - 1]);
					index_hunted--;
				}
			}
			index++;
		}
		*/
	}

private:
	//std::array<Hunted, 1000> hunted;

	int index_hunted = 0;
};
