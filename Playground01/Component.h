#pragma once

#include <functional>
#include "./Handler.h"

// ComponentŠî’ê

class Component {
public:
	const UUID uuid;
	Component() : uuid() {}
	virtual ~Component();
	Component(const Component &) = delete;
	Component & operator=(const Component &) = delete;

	bool operator==(const Component & rhs) const {
		return rhs.uuid == uuid;
	}

	template <typename Element> void execute(const std::function<void(Element &)> func) const;

protected:
	template <typename Element, typename ... Args> Handler<Element> & create(Args ... args) const;
	template <typename Element> void free() const;
};
