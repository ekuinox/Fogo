#pragma once

#include "./LifeCycled.h"
#include "./Renderable.h"
#include "./Updatable.h"
#include "./Component.h"
#include <vector>
#include <memory>

namespace Fogo::Game {
	class Scene : public Component, public LifeCycled, public Renderable, public Updatable {
	public:
		virtual ~Scene() = default;
		void initialize() override;
		void start() override;
		void update() override;
		void render() const override;
		void stop() override;
		void finalize() override;
	};
}
