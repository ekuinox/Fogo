#pragma once

#include "../Component.h"
#include "./LifeCycled.h"

namespace Fogo::Game {
	class Scene : public Component, public LifeCycled {
	public:
		virtual ~Scene() = default;
		void initialize() override;
		void start() override;
		virtual void update();
		virtual void render() const;
		void stop() override;
		void finalize() override;
	};
}
