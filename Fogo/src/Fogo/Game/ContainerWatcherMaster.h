#pragma once

#include "./ContainetWatcher.h"
#include <memory>

namespace Fogo {
	class ContainerWatcherMaster final {
	private:
		std::vector<std::unique_ptr<ContainerWatcherInterface>> __watchers;

	public:
		void watch() const;

		template <typename T, typename Checker = void>
		void create() {
			if (ContainerWatcher<T, Checker>::shared != nullptr) return;
			
			ContainerWatcher<T, Checker>::shared = new ContainerWatcher<T, Checker>();
			__watchers.emplace_back(ContainerWatcher<T, Checker>::shared);
		}

		static ContainerWatcherMaster * shared;
	};
}
