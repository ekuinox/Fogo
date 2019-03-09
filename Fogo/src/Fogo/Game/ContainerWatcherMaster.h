#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "./ContainetWatcher.h"

namespace Fogo {

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
			}
			catch (std::out_of_range & e) {
				return 0;
			}
		}

		static ContainerWatcherMaster * shared;
	};

	template <typename KeyType, typename Hasher>
	ContainerWatcherMaster<KeyType, Hasher> * ContainerWatcherMaster<KeyType, Hasher>::shared = new ContainerWatcherMaster();


	template <typename T>
	struct MakeContainerMaster {
		static constexpr auto Correct = false;
	};

	template <typename ... Args>
	struct MakeContainerMaster<ContainerBase<Args...>> {
		using Container = ContainerBase<Args...>;
		using KeyType = typename Container::KeyType;
		using ValueType = typename Container::ValueType;
		using Hasher = typename Container::hasher;
		using Master = ContainerWatcherMaster<KeyType, Hasher>;

		static constexpr auto Correct = true;

		static void Entry(const KeyType & key) {
			Master::shared->entry<Container>(key);
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
}
