#pragma once

#include "../Game/Component.h"
#include <ostream>

namespace Fogo {
	void DumpComponent(std::ostream& output, Component & parent, int depth, char indentCharacter, char indentLength);
}
