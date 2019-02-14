#pragma once

#include <functional>
#include "./Handler.h"
#include "./Store.h"

namespace Fogo::Game {

	class Component {
	/*

	Componentすべての基底クラス
	UUIDとStoreのラッパ機能のみ

	*/
	public:
		const UUID uuid;
		Component();
		virtual ~Component();
		Component(const Component &) = delete;
		Component & operator=(const Component &) = delete;

		bool operator==(const Component & rhs) const;

		template <typename Element> void execute(const std::function<void(Element &)> func) const;

	protected:
		template <typename Element, typename ... Args> Handler<Element> & create(Args ... args) const;
		template <typename Element> Handler<Element> & bind(Element * element) const;
		template <typename Element> void free() const;
	};

	template<typename Element, typename ... Args>
	Handler<Element> & Component::create(Args ... args) const {
		return Store::Create<Element>(uuid, args ...);
	}

	template<typename Element>
	Handler<Element>& Component::bind(Element * element) const {
		return Store::Bind<Element>(element);
	}

	template<typename Element>
	void Component::execute(const std::function<void(Element&)> func) const {
		Store::Execute<Element>(func, uuid);
	}

	template<typename Element>
	void Component::free() const {
		Store::Free<Element>(uuid);
	}
}
