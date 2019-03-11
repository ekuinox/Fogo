#pragma once

#include "./UUID.h"
#include <unordered_map>
#include "../Utility/Result.h"

namespace Fogo {
	class ComponentTree final : std::unordered_map<UUID /* child */ , UUID /* parent */, UUID::Hash> {
	private:
		ComponentTree();

	public:
		enum class Error {
			NotFound
		};

		void create(const UUID & parent, const UUID & child);
		Result<Error, UUID> getParent(const UUID & child) const;
		std::vector<UUID> getChildren(const UUID & parent) const;

		static ComponentTree * shared;
	};
}
