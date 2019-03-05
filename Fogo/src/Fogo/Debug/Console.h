#pragma once

#include "../Game/Component.h"

namespace Fogo {
	class Console {
		static void Log(Component & parent, int depth, char indentCharacter, char indentLength);
	public:
		static void Log(Component & parent, char indentCharacter = ' ', char indentLength = 2);
	};
}