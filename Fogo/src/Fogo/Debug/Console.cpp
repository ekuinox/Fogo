#include "./Console.h"
#include "./DumpComponent.h"
#include "../Game/Components/Meta.h"
#include <typeinfo>

using Fogo::Console;
using Fogo::Component;

void Console::Log(Component & parent, int depth, char indentCharacter, char indentLength) {
	DumpComponent(std::cout, parent, depth, indentCharacter, indentLength);
}

void Console::Log(Component & parent, char indentCharacter, char indentLength) {
	Log(parent, 0, indentCharacter, indentLength);
}
