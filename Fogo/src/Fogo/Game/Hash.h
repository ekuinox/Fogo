#pragma once

#include <string>
#include <unordered_map>
#include "./ContainerIndexKeyPair.h"

namespace Fogo {
	template <typename Key>
	struct Hash {
		std::size_t operator()(const ContainerIndexKeyPair<Key> & key) const {
			return std::hash<std::string>{}(std::string(reinterpret_cast<const char*>(&key), sizeof ContainerIndexKeyPair<Key>));
		}
	};
}