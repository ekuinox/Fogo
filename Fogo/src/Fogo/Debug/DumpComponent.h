#pragma once

#include "../Game/Component.h"
#include <ostream>

namespace Fogo::Debug {
	void DumpComponent(std::ostream& output, Game::Component & parent, int depth, char indentCharacter, char indentLength);
}