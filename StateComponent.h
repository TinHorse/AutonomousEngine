#pragma once
#include "Components.h"
#include <map>
#include <cassert>

enum Behaviour : std::size_t
{
	idle,
	exploring,
	eating, 
	followTarget,

	returnToOrigin,
	goToOrigin,
	waitForCalm
};

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

	void pushBehaviour(const Behaviour& s, int priority)
	{
		if (!state_machine.empty() && state_machine.top().first == s)
			state_machine.top() = std::make_pair(s, priority);
		else
			state_machine.push(std::make_pair(s, priority));
	}

	void popBehaviour()
	{
		if (!state_machine.empty())
			state_machine.pop();
	}

private:
	std::map<std::string, double> state;
	std::map<std::string, double> bevaviour;

	std::stack<std::pair<Behaviour, int>> state_machine;
};
