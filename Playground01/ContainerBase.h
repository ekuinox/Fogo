#pragma once

#include <unordered_map>

// �V���O���g��������unordered_map

template <typename Key, typename Element, typename ... Args>
class ContainerBase : public std::unordered_map<Key, Element, Args ...> {
private:
	ContainerBase() : std::unordered_map<Key, Element, Args ...>({}) { }

public:
	static ContainerBase shared;
};

template <typename Key, typename Element, typename ... Args>
ContainerBase<Key, Element, Args ...> ContainerBase<Key, Element, Args ...>::shared;