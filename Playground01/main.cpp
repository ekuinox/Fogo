#include <iostream>
#include "./TaskScheduler.h"
#include "../Fogo/src/Fogo/Game/ContainerBase.h"
#include "UUID.h"

/*

C++‚Ì‹@”\‚Æ‚©‘‚¢‚Ä‚İ‚½‚¢‚â‚Â‚ª‚©‚¯‚é‚©‚È‚Ç‚¨‚ê‚ª‚·êŠ@‚»‚Ì‚P

*/

template <typename Key>
struct KeyPair {
	Key key;
	UUID uuid;

	bool operator==(const KeyPair & rhs) const {
		return rhs.key == key && rhs.uuid == uuid;
	}

	bool operator!=(const KeyPair & rhs) const {
		return !(rhs == *this);
	}
};

template <typename Key>
struct Hash {
	std::size_t operator()(const KeyPair<Key> & key) const {
		return std::hash<std::string>{}(std::string(reinterpret_cast<const char*>(&key), sizeof KeyPair<Key>));
	}
};

using Fogo::Game::ContainerBase;

template <class Key, class Val>
using Container = ContainerBase<KeyPair<Key>, Val, Hash<Key>>;

auto main() -> int {
	
	enum class Key {
		Foo,
		Bar,
		Baz
	};

	const UUID uuid;

	const KeyPair<Key> key1 { Key::Foo, uuid };
	const KeyPair<Key> key2 { Key::Bar, uuid };
	const KeyPair<Key> key3 { Key::Foo, uuid };

	using map = Container<Key, int>;

	map::shared[key1] = 100;
	map::shared[key2] = 400;
	map::shared[key3] = 500;

	std::cout << map::shared[key1] << std::endl;
	std::cout << map::shared[key2] << std::endl;
	std::cout << map::shared[key3] << std::endl;


	return 0;
}
