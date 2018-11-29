#pragma once

#include <unordered_map>
#include <functional>

namespace Fogo
{
	template <typename State>
	void match(State state, std::unordered_map<State, std::function<void(void)>> matches)
	{
		matches[state]();
	}
}