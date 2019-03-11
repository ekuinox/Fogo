#include "./ComponentTree.h"

using namespace Fogo;

ComponentTree * ComponentTree::shared = new ComponentTree();

ComponentTree::ComponentTree() : std::unordered_map<UUID, UUID, UUID::Hash>({}) {
	
}

void ComponentTree::create(const UUID& parent, const UUID & child) {
	insert(std::make_pair(child, parent));
}

Result<ComponentTree::Error, UUID> ComponentTree::getParent(const UUID & child) const {
	try {
		return at(child);
	} catch (...) {
		return Error::NotFound;
	}
}

std::vector<UUID> ComponentTree::getChildren(const UUID & parent) const {
	std::vector<UUID> children {};

	for (const auto & [child_, parent_] : *this) {
		if (parent == parent_) {
			children.emplace_back(child_);
		}
	}

	return children;
}
