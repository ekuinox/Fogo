#include "./ContainerWatcherMaster.h"

using namespace Fogo;

void ContainerWatcherMaster::watch() const {
	for (const auto & watcher : __watchers) {
		watcher->watch();
	}
}

ContainerWatcherMaster * ContainerWatcherMaster::shared = new ContainerWatcherMaster();
