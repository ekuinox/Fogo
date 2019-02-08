#pragma once

#include "./Store.h"
#include "./Component.h"

template<typename Element, typename ... Args>
Handler<Element> & Component::create(Args ... args) const {
	return Store::Create<Element>(uuid, args ...);
}

template<typename Element>
void Component::execute(const std::function<void(Element&)> func) const {
	Store::Execute<Element>(func, uuid);
}

template<typename Element>
void Component::free() const {
	Store::Free<Element>(uuid);
}

Component::~Component() {
	Store::Free(uuid);
}
