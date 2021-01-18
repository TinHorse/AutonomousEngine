#pragma once
#include <stack>
#include <set>

template<typename BV>
class StateMachine
{
public:
	StateMachine() = default;
	bool push(BV bv, int priority)
	{
		if (state.empty())
		{
			state.push(std::make_pair(bv, priority));
			state_tracker.insert(bv);
			return true;
		}
		else if (priority >= state.top().second)
		{
			if (state_tracker.find(bv) == state_tracker.end())
			{
				state.push(std::make_pair(bv, priority));
				state_tracker.insert(bv);
				return true;
			}
			else
			{
				while (!state.empty() && state.top().first != bv)
				{
					temp.push(state.top());
					state.pop();
				}
				state.pop();
				while (!temp.empty())
				{
					state.push(temp.top());
					temp.pop();
				}
				state.push(std::make_pair(bv, priority));
				return true;
			}
		}
		return false;
	}

	void pop()
	{
		if (!state.empty())
		{
			state_tracker.erase(state.top().first);
			state.pop();
		}
	}

	const BV& current()
	{
		if (!state.empty())
		{
			return state.top().first;
		}
		return deflt;
	}

	const int& currentPriority()
	{
		if (!state.empty())
		{
			return state.top().second;
		}
		return 0;
	}



private:
	std::stack<std::pair<BV, int>> state;
	std::stack<std::pair<BV, int>> temp;
	BV deflt;
	std::set<BV> state_tracker;

};