#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Starter : public CallableComponent {
	public:
		Starter(std::function<void(void)> function);
	};
}
