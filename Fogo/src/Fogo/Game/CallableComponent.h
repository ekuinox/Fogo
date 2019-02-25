#pragma once

#include "./Component.h"
#include <functional>

namespace Fogo::Game {
	class CallableComponent : public Component {
		std::function<void(void)> __function;
	public:
		CallableComponent(std::function<void(void)> function);
		void operator()() const;
	};
}