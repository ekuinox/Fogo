#pragma once

#include "../Utility/Store.h"
#include "../Utility/HelperFunctions.h"

namespace Fogo::Game {
	class ComponentInterface {
	public:
		enum class State {
			Created, // new
			Initialized, // initialize
			Started, // start
		};

		const std::string guid;
		const Utility::Store store;

	protected:
		State __state = State::Created;
	public:
		ComponentInterface() : guid(Utility::GenerateGUID()), store(guid) { }
		virtual ~ComponentInterface() = default;
		virtual auto initialize() -> void { __state = State::Initialized; }
		virtual auto start() -> void { __state = State::Started; }
		virtual auto update() -> void { /* do nothing */ }
		virtual auto render() const -> void { /* do nothing */ }
		virtual auto stop() -> void { __state = State::Initialized; }
		virtual auto finalize() -> void { __state = State::Created; }
		virtual auto getState() -> State { return __state; }
	};
}
