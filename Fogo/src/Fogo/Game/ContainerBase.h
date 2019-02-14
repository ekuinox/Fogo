#pragma once

#include <unordered_map>

namespace Fogo::Game {

	template <typename Key, typename Element, typename ... Args>
	class ContainerBase : public std::unordered_map<Key, Element, Args ...> {
	/*
		Store���������邽�߁C�V���O���g���ȃR���e�i��p�ӂ��Ă���
	*/

	private:
		ContainerBase() : std::unordered_map<Key, Element, Args ...>({}) { }

	public:
		static ContainerBase shared;
	};

	template <typename Key, typename Element, typename ... Args>
	ContainerBase<Key, Element, Args ...> ContainerBase<Key, Element, Args ...>::shared;
}
