#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Initializer : public CallableComponent {
		Initializer(std::function<void(void)> function);
	};
}
