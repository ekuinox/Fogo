#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Stopper : public CallableComponent {
	public:
		Stopper(std::function<void(void)> function);
	};
}
