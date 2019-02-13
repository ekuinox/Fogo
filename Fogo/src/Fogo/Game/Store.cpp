#include "./Store.h"

using Fogo::Game::Store;
using Fogo::Game::UUID;

void Store::Free(const UUID & parentId) {
	Free<Updatable, Renderable, LifeCycled, Component>(parentId);
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();