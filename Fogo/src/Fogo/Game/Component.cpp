#include "./Component.h"
#include "./System.h"

using Fogo::Result;
using Fogo::Component;
using Fogo::Store;
using Fogo::Handler;
using Fogo::Scene;
using Fogo::System;

Component::Component() : uuid() {
}

bool Component::operator==(const Component & rhs) const {
	return rhs.uuid == uuid;
}

Result<Store::Error, Handler<Scene>> Component::getMyScene() const {
	if (const auto & scene = getParent<Scene>()) {
		return scene; // 親がすでにSceneならそのまま返却する
	}

	if (const auto & parent = getParent()) {
		if (parent->uuid != System::GetInstance().uuid) {
			return parent->getMyScene(); // 親の更に親で試す
		}
	}

	return Store::Error::NotExist;
}

Component::~Component() {
	Store::Free(uuid);
}
