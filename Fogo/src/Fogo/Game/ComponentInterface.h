#pragma once

namespace Fogo::Game {
	class ComponentInterface {
	public:
		virtual ~ComponentInterface() = default;
		virtual auto update() -> void = 0;
		virtual auto render() const -> void { /* do nothing */ }
	};
}
