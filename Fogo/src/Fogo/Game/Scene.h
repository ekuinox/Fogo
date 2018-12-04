#pragma once

#include "./ComponentInterface.h"
#include <vector>
#include <memory>
#include "../Graphics.h"

namespace Fogo::Game {
	class Scene {
	public:
		std::vector<std::shared_ptr<ComponentInterface>> components;
		auto update() -> void;
		auto render() const -> void;
	};
}
