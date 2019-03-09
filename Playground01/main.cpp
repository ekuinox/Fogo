#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include "../Fogo/src/Fogo/Game/ContainerBase.h"
#include "../Fogo/src/Fogo/Game/ContainetWatcher.h"
#include "../Fogo/src/Fogo/Game/UUID.h"

/*

C++‚Ì‹@”\‚Æ‚©‘‚¢‚Ä‚İ‚½‚¢‚â‚Â‚ª‚©‚¯‚é‚©‚È‚Ç‚¨‚ê‚ª‚·êŠ@‚»‚Ì‚P

*/

template <typename KeyType>
class ContainerWatcherInterface {
public:
	virtual void erase(const KeyType & uuid) const = 0;
};

template <typename Container>
class ContainerWatcher : public ContainerWatcherInterface<typename Container::KeyType> {
public:
	void erase(const typename Container::KeyType & uuid) const override {
		Container::shared.erase(uuid);
	}

	static ContainerWatcher * shared;
};

template <typename Container>
ContainerWatcher<Container> * ContainerWatcher<Container>::shared = new ContainerWatcher<Container>();

template <typename KeyType, typename Hasher = std::hash<KeyType>>
class ContainerWatcherMaster {
	std::unordered_map<KeyType, std::vector<ContainerWatcherInterface<KeyType>*>, Hasher> __watchers;

public:
	template <typename Container>
	void entry(const KeyType & uuid) {
		// ˆê“x‚«‚è‚µ‚©ŒÄ‚Î‚ê‚È‚¢‚Æ‚µ‚Ä (d•¡‚Í‚È‚¢‚à‚Ì‚Æ‚·‚é)
		__watchers[uuid].emplace_back(ContainerWatcher<Container>::shared);
	}

	void erase(const KeyType & uuid) {
		for (const auto & watcher : __watchers[uuid]) {
			watcher->erase(uuid);
		}
		__watchers.erase(uuid);
	}

	// “o˜^‚³‚ê‚Ä‚¢‚ékey‚Ì”
	std::size_t count() const {
		return __watchers.size();
	}

	// Key‚É“o˜^‚³‚ê‚Ä‚¢‚éWatcher‚Ì”
	std::size_t count(const KeyType & uuid) const {
		try {
			return __watchers.at(uuid).size();
		} catch (std::out_of_range & e) {
			return 0;
		}
	}

	static ContainerWatcherMaster * shared;
};

template <typename KeyType, typename Hasher>
ContainerWatcherMaster<KeyType, Hasher> * ContainerWatcherMaster<KeyType, Hasher>::shared = new ContainerWatcherMaster();

template <typename T>
struct MakeMaster {
	static constexpr auto Correct = false;
};

template <typename ... Args>
struct MakeMaster<Fogo::ContainerBase<Args...>> {
	using Container = Fogo::ContainerBase<Args...>;
	using KeyType = typename Container::KeyType;
	using ValueType = typename Container::ValueType;
	using Hasher = typename Container::hasher;
	using Master = ContainerWatcherMaster<KeyType, Hasher>;

	static constexpr auto Correct = true;

	static void Entry(const KeyType & key) {
		Master::shared->template entry<Container>(key);
	}

	static void Erase(const KeyType & key) {
		Master::shared->erase(key);
	}

	static std::size_t Count() {
		return Master::shared->count();
	}

	static std::size_t Count(const KeyType & key) {
		return Master::shared->count(key);
	}

	Master * operator()() const {
		return Master::shared;
	}
};

template <typename T>
using Container = Fogo::ContainerBase<Fogo::UUID, int, Fogo::UUID::Hash>;

auto main() -> int {

	const auto uuid = Fogo::UUID();

	MakeMaster<Container<int>>::Entry(uuid);
	MakeMaster<Container<float>>::Entry(uuid);
	MakeMaster<Container<double>>::Entry(uuid);
	MakeMaster<Container<int>>::Entry(Fogo::UUID());

	std::cout << MakeMaster<Container<int>>::Count() << std::endl;

	std::cout << MakeMaster<Container<int>>::Count(uuid) << std::endl;

	std::cout << "release " << uuid << std::endl;
	MakeMaster<Container<int>>::Erase(uuid);

	std::cout << MakeMaster<Container<int>>::Count() << std::endl;

	std::cout << MakeMaster<Container<int>>::Count(uuid) << std::endl;

	std::cout << MakeMaster<Container<int>>::Count(Fogo::UUID());

	return 0;
}
