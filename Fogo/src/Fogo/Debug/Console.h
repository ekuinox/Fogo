#pragma once

#include "../Game/Component.h"

namespace Fogo::Debug {
	class Console {
		static void Log(Game::Component & parent, int depth, char indentCharacter, char indentLength);
	public:
		static void Log(Game::Component & parent, char indentCharacter = ' ', char indentLength = 2);
	};
}