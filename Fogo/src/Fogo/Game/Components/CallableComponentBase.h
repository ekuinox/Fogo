#pragma once

#include <functional>
#include "../Component.h"

namespace Fogo::Game {
	template <int N = 0, typename ... Args>
	class CallableComponentBase : public Component {
		using Function = std::function<void(Args...)>;
		Function __function;

	public:
		CallableComponentBase(Function function) : __function(std::move(function)) {

		}
		void operator()(Args ... args) const {
			__function(args...);
		}
	};
}
