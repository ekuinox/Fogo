#include "./Store.h"
#include "./Components/Scene.h"

using Fogo::Store;
using Fogo::UUID;

void Store::Free(const UUID & parentId) {
	std::vector<UUID> children = ComponentTree::shared->getChildren(parentId);

	for (const auto & child : children) {
		Handler<Component>::Create(Container<Component>::shared[child]).release();
		MakeContainerMaster<Container<Component>>::Erase(child);
	}
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();
