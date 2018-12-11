#pragma once

#include "./ComponentInterface.h"
#include <vector>
#include <memory>

namespace Fogo::Game {
	class Scene {
	public:
		virtual ~Scene() = default;
		std::vector<std::shared_ptr<ComponentInterface>> components;
		virtual auto initialize() -> void;
		virtual auto update() -> void;
		virtual auto render() const -> void;
		virtual auto finalize() -> void;
	};
}
