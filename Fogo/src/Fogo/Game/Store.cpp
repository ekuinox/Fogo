#include "./Store.h"
#include "./Components/Scene.h"

using Fogo::Store;
using Fogo::UUID;

void Store::Free(const UUID & parentId) {
	for (const auto & child : ComponentTree::shared->getChildren(parentId)) {
		try {
			auto & component = Container<Component>::shared.at(child);
			delete component;
			component = nullptr;
			MakeContainerMaster<Container<Component>>::Erase(child);
		} catch (...) {}
	}
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();
