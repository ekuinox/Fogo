#include "Console.h"
#include "../Game/Components/Meta.h"
#include <typeinfo>

using Fogo::Debug::Console;
using Fogo::Game::Component;

void Console::Log(Component & parent, int depth, char indentCharacter, char indentLength) {
	std::string indent1, indent2;
	for (auto i = 0; i < indentLength; ++i) {
		indent1 += indentCharacter;
	}
	for (auto i = 0; i < depth; ++i) {
		indent2 += indent1;
	}
	std::cout << indent2 << typeid(parent).name() << ": {" << std::endl;
	std::cout << indent2 << indent1 << "uuid: " << parent.uuid << "," << std::endl;
	if (const auto & meta = parent.get<Fogo::Game::Meta>()) {
		std::cout << indent2 << indent1 << "name: " << meta->name << "," << std::endl;
		std::cout << indent2 << indent1 << "tag: " << meta->tag << "," << std::endl;
		std::cout << indent2 << indent1 << "layer: " << meta->layer << "," << std::endl;
	}
	parent.execute<Component>([&](Component & component) {
		Log(component, depth + 1, indentCharacter, indentLength);
	});
	std::cout << indent2 << "}," << std::endl;
}

void Console::Log(Component & parent, char indentCharacter, char indentLength) {
	Log(parent, 0, indentCharacter, indentLength);
}
