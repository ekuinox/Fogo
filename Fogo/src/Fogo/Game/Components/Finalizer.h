#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Finalizer : public CallableComponent {
		Finalizer(std::function<void(void)> function);
	};
}
