#pragma once

#include <vector>
#include "./ContainerBase.h"

namespace Fogo {

	class ContainerWatcherInterface {
	public:
		virtual void watch() const = 0;
	};

	template <typename Container, typename Checker = void>
	class ContainerWatcher : public ContainerWatcherInterface {
	public:
		void watch() const override {
			static_assert(IsContainerBase<Container>::value);

			std::vector<typename Container::KeyType> keys{};
			
			for (const auto &[key, value] : Container::shared) {
				if constexpr (std::is_void<Checker>()) {
					if (value == nullptr) keys.emplace_back(key);
				} else {
					if (!Checker()(value)) keys.emplace_back(key);
				}
			}

			for (const auto & key : keys) {
				Container::shared.erase(key);
			}
		}

		static ContainerWatcher * shared;
	};

	template <typename Container, typename Checker>
	ContainerWatcher<Container, Checker> * ContainerWatcher<Container, Checker>::shared = nullptr;

}
