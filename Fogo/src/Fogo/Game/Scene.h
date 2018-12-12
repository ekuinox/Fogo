#pragma once

#include "./ComponentInterface.h"
#include <vector>
#include <memory>

namespace Fogo::Game {
	class Scene : public ComponentInterface {
	protected:
		std::vector<std::shared_ptr<ComponentInterface>> components;
	public:
		virtual ~Scene() = default;
		auto initialize() -> void override;
		auto start() -> void override;
		auto update() -> void override;
		auto render() const -> void override;
		auto stop() -> void override;
		auto finalize() -> void override;
	};
}
