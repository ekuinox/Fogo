#pragma once

#include <unordered_map>
#include <type_traits>
#include "./Handler.h"
#include "./ContainerBase.h"

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
	static const std::size_t GetSize();

	static const std::size_t GetSize();

};
