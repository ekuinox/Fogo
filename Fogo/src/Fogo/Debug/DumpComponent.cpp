#include "./DumpComponent.h"
#include "../Game/Components/Meta.h"

using Fogo::Game::Component;
using Fogo::Game::Meta;

void Fogo::Debug::DumpComponent(std::ostream & output, Component & parent, int depth, char indentCharacter, char indentLength) {
	std::string indent1, indent2;
	for (auto i = 0; i < indentLength; ++i) {
		indent1 += indentCharacter;
	}
	for (auto i = 0; i < depth; ++i) {
		indent2 += indent1;
	}
	output << indent2 << "\"" << typeid(parent).name() << "\": {" << std::endl;
	output << indent2 << indent1 << "uuid: " << parent.uuid << "," << std::endl;
	if (const auto & meta = parent.get<Fogo::Game::Meta>()) {
		output << indent2 << indent1 << "name: \"" << meta->name << "\"," << std::endl;
		output << indent2 << indent1 << "tag: \"" << meta->tag << "\"," << std::endl;
		output << indent2 << indent1 << "layer: " << meta->layer << "," << std::endl;
	}
	parent.execute<Component>([&](Component & component) {
		DumpComponent(output, component, depth + 1, indentCharacter, indentLength);
	});
	output << indent2 << "}," << std::endl;
}
