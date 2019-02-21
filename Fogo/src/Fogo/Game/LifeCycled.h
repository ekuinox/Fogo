#pragma once

#include "../Utility/HelperFunctions.h"

namespace Fogo::Game {
	class LifeCycled {
	public:
		enum class State {
			Created, // new
			Initialized, // initialize
			Started, // start
		};

	protected:
		State __state = State::Created;
	public:
		virtual auto initialize() -> void { __state = State::Initialized; }
		virtual auto start() -> void { __state = State::Started; }
		virtual auto stop() -> void { __state = State::Initialized; }
		virtual auto finalize() -> void { __state = State::Created; }
		virtual auto getState() -> State { return __state; }

		template <typename T>
		static constexpr bool IsDerived() {
			return std::is_base_of<LifeCycled, T>();
		}

		template <typename T>
		static constexpr bool IsSame() {
			return std::is_same<LifeCycled, T>();
		}
	};
}
