#pragma once

#include "./CallableComponent.h"

namespace Fogo::Game {
	class Renderer : public CallableComponent {
	public:
		Renderer(std::function<void(void)> function);
	};
}