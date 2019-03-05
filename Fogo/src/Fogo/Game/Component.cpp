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
		return scene; // �e�����ł�Scene�Ȃ炻�̂܂ܕԋp����
	}

	if (const auto & parent = getParent()) {
		if (parent->uuid != System::GetInstance().uuid) {
			return parent->getMyScene(); // �e�̍X�ɐe�Ŏ���
		}
	}

	return Store::Error::NotExist;
}

Component::~Component() {
	Store::Free(uuid);
}
