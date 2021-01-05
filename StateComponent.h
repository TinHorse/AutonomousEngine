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

	void initS(std::string st, const double& value)
	{
		assert(state.find(st) == state.end() && "attempting to INIT state that already exists");
		state[st] = value;
	}

	const double& getS(std::string st)
	{
		assert(state.find(st) != state.end() && "attempting to GET state that doesnt exist");
		return state[st];
	}

	void setS(std::string st, const double& value)
	{
		assert(state.find(st) != state.end() && "attempting to SET a state that doesnt exist");
		state[st] = value;
	}

	void addS(std::string st, const double& value)
	{
		assert(state.find(st) != state.end() && "attempting to ADD TO a state that doesnt exist ");
		state[st] += value;
	}



	void initB(std::string bv, const double& value)
	{
		assert(bevaviour.find(bv) == bevaviour.end() && "attempting to INIT behaviour that already exists");
		bevaviour[bv] = value;
	}

	const double& getB(std::string bv)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to GET behaviour that doesnt exist");
		return bevaviour[bv];
	}

	void setB(std::string bv, const double& value)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to SET behaviour that doesnt exist");
		bevaviour[bv] = value;
	}

	void addB(std::string bv, const double& value)
	{
		assert(bevaviour.find(bv) != bevaviour.end() && "attempting to ADD TO behaviour that doesnt exist");
		bevaviour[bv] += value;
	}

	Entity* getTargetEntity()
	{
		return target_entity;
	}

private:
	std::map<std::string, double> state;
	std::map<std::string, double> bevaviour;
	Entity* target_entity;
};