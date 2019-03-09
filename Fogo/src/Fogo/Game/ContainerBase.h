#pragma once

#include <unordered_map>

namespace Fogo {

	template <typename Key, typename Element, typename ... Args>
	class ContainerBase : public std::unordered_map<Key, Element, Args ...> {
	/*
		Storeを実装するため，シングルトンなコンテナを用意している
	*/

	private:
		ContainerBase() : std::unordered_map<Key, Element, Args ...>({}) { }

	public:
		using KeyType = Key;
		using ValueType = Element;

		static ContainerBase shared;
	};

	template <typename Key, typename Element, typename ... Args>
	ContainerBase<Key, Element, Args ...> ContainerBase<Key, Element, Args ...>::shared;
}
