#include "./Component.h"

using Fogo::Game::Component;

Component::Component() : uuid() {
}

bool Component::operator==(const Component & rhs) const {
	return rhs.uuid == uuid;
}

Component::~Component() {
	Store::Free(uuid);
}
