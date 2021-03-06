#pragma once

#include "../Component.h"

namespace Fogo {
	class Scene : public Component {
	public:
		enum class State {
			Created, // new
			Initialized, // initialize
			Started, // start
		};

	protected:
		State __state;

	public:
		Scene();
		virtual void initialize();
		virtual void start();
		virtual void stop();
		virtual void finalize();
		virtual void render() const;
		virtual void update();
		State getState() const;
	};
}
