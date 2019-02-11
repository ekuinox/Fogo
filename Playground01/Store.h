#pragma once

#include <unordered_map>
#include <type_traits>
#include <functional>
#include "./Handler.h"
#include "./ContainerBase.h"
#include "./Renderable.h"
#include "./Updatable.h"
#include "./Result.h"

class Component;

class Store final {
private:
	template <typename Element>
	using Container = ContainerBase<UUID, Handler<Element>, UUID::Hash>;

	template <typename Element>
	static void Insert(Element * element, const UUID & parentId);
	
	template <typename Element>
	static void Insert(Element * element, const UUID & parentId, const UUID & uuid);

public:
	static const UUID rootId;

	// 要件を満たした型か確認する
	template <typename Element>
	static constexpr bool IsCorrectElement();

	// ポインタをバインドする
	template <typename Element>
	static Handler<Element> & Bind(Element * element, const UUID & parentId = rootId);

	// 構築する
	template <typename Element, typename ... Args>
	static Handler<Element> & Create(const UUID & parentId = rootId, Args ... args);

	// UUIDを用いてComponentを取得する
	template <typename Element>
	static Handler<Element> & Get(const UUID & uuid);

	// 指定したキーで管理しているComponentを取得する
	template <typename Element, typename Key>
	static Element & Get(const Key & key);
	
	// 指定したComponentの親を取得する
	template <typename Element = Component>
	static Handler<Element> & GetParent(const UUID & uuid);

	// コンテナの要素に対して実行する
	template <typename Element>
	static void Execute(const std::function<void(Element &)> & func);

	// コンテナの任意の親IDを持った要素に対して実行する
	template <typename Element>
	static void Execute(const std::function<void(Element &)> & func, const UUID & parentId);

	// 管理を放棄する
	template <typename Element>
	static Element * Detach(const UUID & uuid);

	// コンテナの中身を開放していく
	template <typename Element, typename ... Elements>
	static void Free(const UUID & parentId = rootId);

	static void Free(const UUID & parentId = rootId);

	// コンテナのサイズを知る
	template <typename Element>
	static std::size_t GetSize();

	static std::size_t GetSize();

};

template <typename Element>
void Store::Insert(Element * element, const UUID & parentId) {
	Container<Element>::shared.insert(std::make_pair(element->uuid, Handler<Element>::Create(element, parentId)));
}

template <typename Element>
void Store::Insert(Element * element, const UUID & parentId, const UUID & uuid) {
	Container<Element>::shared.insert(std::make_pair(uuid, Handler<Element>::Create(element, parentId)));
}

template <typename Element>
constexpr bool Store::IsCorrectElement() {
	return
		std::is_base_of<Component, Element>()
		|| std::is_same<Element, Updatable>()
		|| std::is_same<Element, Renderable>()
		;
}

template <typename Element>
Handler<Element> & Store::Bind(Element * element, const UUID & parentId) {
	static_assert(IsCorrectElement<Element>());

	Insert<Element>(element, parentId);

	Insert<Component>(element, parentId);

	if constexpr (Updatable::IsDerived<Element>()) Insert<Updatable>(element, parentId, element->uuid);

	if constexpr (Renderable::IsDerived<Element>()) Insert<Renderable>(element, parentId, element->uuid);

	return Get<Element>(element->uuid);
}

template <typename Element, typename ... Args>
Handler<Element> & Store::Create(const UUID & parentId, Args ... args) {
	return Bind(new Element(args...), parentId);
}

template <typename Element>
Handler<Element> & Store::Get(const UUID & uuid) {
	static_assert(IsCorrectElement<Element>());
	return Container<Element>::shared.at(uuid);
}

template <typename Element, typename Key>
Element & Store::Get(const Key & key) {
	static_assert(IsCorrectElement<Element>());
	return *ContainerBase<Key, Element *>::shared.at(key);
}

template <typename Element>
Handler<Element> & Store::GetParent(const UUID & uuid) {
	static_assert(IsCorrectElement<Element>());
	auto parentId = Container<Element>::shared.at(uuid).parentId;
	return Container<Element>::shared.at(parentId == rootId ? uuid : parentId);
}

template <typename Element>
void Store::Execute(const std::function<void(Element &)> & func) {
	static_assert(IsCorrectElement<Element>());
	for (const auto &[_, element] : Container<Element>::shared) {
		func(*(element.second));
	}
}

template <typename Element>
void Store::Execute(const std::function<void(Element &)> & func, const UUID & parentId) {
	static_assert(IsCorrectElement<Element>());
	for (const auto &[_, element] : Container<Element>::shared) {
		if (element.parentId == parentId) func(*(element.element));
	}
}

template <typename Element>
Element * Store::Detach(const UUID & uuid) {
	static_assert(IsCorrectElement<Element>());

	const auto element = Container<Element>::shared.at(uuid).element;

	Container<Element>::shared.erase(uuid);

	Container<Component>::shared.erase(uuid);

	if constexpr (std::is_base_of<Updatable, Element>()) {
		Container<Updatable>::shared.erase(uuid);
	}

	if constexpr (std::is_base_of<Renderable, Element>()) {
		Container<Renderable>::shared.erase(uuid);
	}

	return element;
}

template <typename Element, typename ... Elements>
void Store::Free(const UUID & parentId) {
	static_assert(IsCorrectElement<Element>());

	std::vector<UUID> uuids{};

	for (const auto &[uuid, element] : Container<Element>::shared) {
		if (element.parentId == parentId) {
			uuids.emplace_back(uuid);
		}
	}

	for (const auto & uuid : uuids) {
		if constexpr (!(std::is_same<Element, Updatable>() || std::is_same<Element, Renderable>())) {
			delete Container<Element>::shared[uuid].element;
			Container<Element>::shared[uuid].element = nullptr;
		}

		Container<Element>::shared.erase(uuid);
	}

	if constexpr (sizeof...(Elements) > 0) {
		Free<Elements...>(parentId);
	}
}

void Store::Free(const UUID & parentId) {
	Free<Updatable, Renderable, Component>(parentId);
}

template <typename Element>
std::size_t Store::GetSize() {
	return Container<Element>::shared.size();
}

std::size_t Store::GetSize() {
	return GetSize<Component>();
}

const UUID Store::rootId = UUID();
