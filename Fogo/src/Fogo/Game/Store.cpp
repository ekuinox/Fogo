#include "./Store.h"
#include "./Components/Scene.h"

using Fogo::Store;
using Fogo::UUID;

void Store::Free(const UUID & parentId) {
	Free<Scene, Component>(parentId);
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();
