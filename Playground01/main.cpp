#include <iostream>
#include <typeinfo>
#include "../Fogo/src/Fogo/Game/ContainerBase.h"

/*

C++の機能とか書いてみたいやつがかけるかなどおれが試す場所　その１

*/

class WatcherInterface {
public:
	virtual void watch() const = 0;
};

template <typename T>
class Watcher : public WatcherInterface {
public:
	void watch() const override {
		std::cout << typeid(T).name() << " watching 中！" << std::endl;
		std::vector<const char *> keys {};
		for (const auto & [key, value] : Fogo::ContainerBase<const char *, T*>::shared) {
			if (value == nullptr) {
				keys.emplace_back(key);
			}
		}

		for (const auto & key : keys) {
			Fogo::ContainerBase<const char *, T*>::shared.erase(key);
		}
	}

	static Watcher * shared;
};

template <typename T>
Watcher<T> * Watcher<T>::shared = nullptr;

class WatcherMaster final {
private:
	std::vector<std::unique_ptr<WatcherInterface>> __watchers;

public:
	void watch() const {
		for (const auto & watcher : __watchers) {
			watcher->watch();
		}
	}

	template <typename T>
	void create() {
		if (Watcher<T>::shared != nullptr) {
			return;
		}
		Watcher<T>::shared = new Watcher<T>();
		__watchers.emplace_back(Watcher<T>::shared);
	}

	static WatcherMaster * shared;
};

WatcherMaster * WatcherMaster::shared = new WatcherMaster();

auto main() -> int {

	WatcherMaster::shared->create<int>();
	WatcherMaster::shared->create<int>();
	WatcherMaster::shared->create<int>();
	WatcherMaster::shared->create<float>();
	WatcherMaster::shared->create<float>();
	WatcherMaster::shared->create<double>();

	WatcherMaster::shared->watch();


	return 0;
}
