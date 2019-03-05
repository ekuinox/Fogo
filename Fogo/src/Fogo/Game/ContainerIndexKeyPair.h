#pragma once

#include "./UUID.h"

namespace Fogo {
	template <typename Key>
	struct ContainerIndexKeyPair {
		Key key;
		UUID uuid;
		bool operator==(const ContainerIndexKeyPair & rhs) const {
			return rhs.key == key && rhs.uuid == uuid;
		}
		bool operator!=(const ContainerIndexKeyPair & rhs) const {
			return !(rhs == *this);
		}
	};
}