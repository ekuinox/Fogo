#pragma once

#include <unordered_map>

// ƒVƒ“ƒOƒ‹ƒgƒ“‰»‚µ‚½unordered_map

namespace Fogo::Game {

	template <typename Key, typename Element, typename ... Args>
	class ContainerBase : public std::unordered_map<Key, Element, Args ...> {
	private:
		ContainerBase() : std::unordered_map<Key, Element, Args ...>({}) { }

	public:
		static ContainerBase shared;
	};

	template <typename Key, typename Element, typename ... Args>
	ContainerBase<Key, Element, Args ...> ContainerBase<Key, Element, Args ...>::shared;
}
