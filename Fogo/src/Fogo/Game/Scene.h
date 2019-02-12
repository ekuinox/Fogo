#pragma once

#include "./ComponentInterface.h"
#include "./Renderable.h"
#include "./Updatable.h"
#include <vector>
#include <memory>

namespace Fogo::Game {
	class Scene : public ComponentInterface, Renderable, Updatable {
	public:
		virtual ~Scene() = default;
		void update() override;
		void render() const override;
	};
}
