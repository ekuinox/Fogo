#pragma once
#include <wrl/client.h>
#include "../Graphics.h"

namespace Fogo::Game {
	class ComponentInterface {
	public:
		virtual ~ComponentInterface() = default;
		virtual auto update() -> void = 0;
		virtual auto render() const -> void = 0;
	};
}
