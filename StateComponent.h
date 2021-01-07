#pragma once
#include "Components.h"
#include <map>
#include <cassert>

class StateComponent : public Component
{
public:
	StateComponent() {}
	void Init() override
	{
		
	}
	
	void Update() override
	{
		
	}


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


	// BEHAVIOUR

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

	std::map<std::string, Entity*> targetEntities;
};
