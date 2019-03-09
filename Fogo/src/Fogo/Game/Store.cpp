#include "./Store.h"
#include "./Components/Scene.h"

using Fogo::Store;
using Fogo::UUID;

void Store::Free(const UUID & parentId) {
	std::vector<UUID> children {};
	for (const auto &[uuid, element] : Container<Component>::shared) {
		if (element.getParentUUID() == parentId) {
			children.emplace_back(uuid);
		}
	}
	for (const auto & child : children) {
		Container<Component>::shared[child].release();
		MakeContainerMaster<Container<Component>>::Erase(child);
	}
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();
