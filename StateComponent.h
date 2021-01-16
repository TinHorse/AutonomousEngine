#pragma once
#include "Components.h"
#include <map>
#include <cassert>
#include <set>
#include <stack>

class StateComponent : public Component
{
public:
	StateComponent() {}
	StateComponent(std::string _tag) {tag = _tag;}
	void Init() override
	{
		
	}
	
	void Update() override
	{
		
	}

	std::string tag;

	// STATE

	void initS(const std::string& st, const double& value)
	{
		assert(state.find(st) == state.end() && "attempting to INIT state that already exists");
		state[st] = value;
	}

	const double& getS(const std::string& st)
	{
		assert(state.find(st) != state.end() && "attempting to GET state that doesnt exist");
		return state[st];
	}

	void setS(const std::string& st, const double& value)
	{
		assert(state.find(st) != state.end() && "attempting to SET a state that doesnt exist");
		state[st] = value;
	}

	void addS(const std::string& st, const double& value)
	{
		assert(state.find(st) != state.end() && "attempting to ADD TO a state that doesnt exist ");
		state[st] += value;
	}


	// BEHAVIOUR VARIABLES

	void initB(const std::string& bv, const double& value)
	{
		assert(bevaviour.find(bv) == bevaviour.end() && "attempting to INIT behaviour that already exists");
		bevaviour[bv] = value;
	}

	const double& getB(const std::string& bv)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to GET behaviour that doesnt exist");
		return bevaviour[bv];
	}

	void setB(const std::string& bv, const double& value)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to SET behaviour that doesnt exist");
		bevaviour[bv] = value;
	}

	void addB(const std::string& bv, const double& value)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to ADD TO behaviour that doesnt exist");
		bevaviour[bv] += value;
	}

	// BEHAVIOURS

	const Behaviour& currentBehaviour()
	{
		if (!state_machine.empty())
			return state_machine.top().first;
		else
			return idle;
	}

	const bool& currentPriority()
	{
		if (!state_machine.empty())
			return state_machine.top().second;
		else
			return 0;
	}

	void pushBehaviour(const Behaviour& s, int priority, bool forcePush = false)
	{
		if (!state_machine.empty())
		{
			if (forcePush || currentPriority() <= priority)
				if (state_machine.top().first != s)
					state_machine.push(std::make_pair(s, priority));
				else
					state_machine.top() = std::make_pair(s, priority);
		}
		else
			state_machine.push(std::make_pair(s, priority));
	}

	void popBehaviour()
	{
		if (!state_machine.empty())
			state_machine.pop();
	}


	// TARGETS

	void initTarget(const std::string& t, Entity* entity)
	{
		assert(targetEntities.find(t) == targetEntities.end() && "attempting to INIT target that already exists");
		targetEntities[t] = entity;
	}

	Entity* getTarget(const std::string& t)
	{
		assert(targetEntities.find(t) != targetEntities.end() && "attempting to GET target that doesnt exist");
		return targetEntities[t];
	}

	void setTarget(const std::string& t, Entity* entity)
	{
		assert(targetEntities.find(t) != targetEntities.end() && "attempting to SET target that doesnt exist");
		targetEntities[t] = entity;
	}

	void UpdateTargetEntities(std::set<Entity*>& deleted_entities)
	{
		for (auto& entity : deleted_entities)
		{
			for (auto& targets : targetEntities)
			{
				if (targets.second == entity)
				{
					targets.second = nullptr;
				}
			}
		}
	}

private:
	std::map<std::string, double> state;
	std::map<std::string, double> bevaviour;

	std::stack<std::pair<Behaviour, int>> state_machine;

	std::map<std::string, Entity*> targetEntities;
};
