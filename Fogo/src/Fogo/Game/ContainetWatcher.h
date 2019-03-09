#pragma once

namespace Fogo {

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
}
