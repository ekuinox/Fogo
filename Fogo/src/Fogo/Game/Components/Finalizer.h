#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Finalizer : public CallableComponent {
	public:
		Finalizer(std::function<void(void)> function);
	};
}
