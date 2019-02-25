#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Updater : public CallableComponent {
	public:
		Updater(std::function<void(void)> function);
	};
}
