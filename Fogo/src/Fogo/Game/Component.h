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

		// uuidを用いて比較する
		bool operator==(const Component & rhs) const;

		// ラムダ式を使って実行する
		template <typename Element>
		void execute(const std::function<void(Element &)> func) const;

		// インデックス用キーを用いて子インスタンスを取得する
		template <typename Element, typename Key>
		Utility::Result<Store::Error, Element*> get(Key key) const;

		// UUIDを用いて子インスタンスのハンドラを取得する
		template <typename Element>
		Utility::Result<Store::Error, Handler<Element>> get(const UUID & uuid) const;

		// 一つの子インスタンスのハンドラを取得する
		// 単一であることを保証するために、0か複数の場合はエラーが返る
		template <typename Element>
		Utility::Result<Store::Error, Handler<Element>> get() const;

		// 自分を持っているSceneを取得する
		Utility::Result<Store::Error, Handler<Scene>> getMyScene() const;
		
		// 親インスタンスのハンドラを取得をする
		template <typename Element = Component>
		Utility::Result<Store::Error, Handler<Element>> getParent() const;

		// 子インスタンスを生成する
		template <typename Element, typename ... Args>
		Handler<Element> & create(Args ... args) const;

		// 子インスタンスとしてバインドする
		template <typename Element>
		Handler<Element> & bind(Element * element) const;

		// 子インスタンスをすべて解放する
		template <typename Element>
		void free() const;
	};

	template<typename Element, typename ... Args>
	Handler<Element> & Component::create(Args ... args) const {
		return Store::Create<Element>(uuid, args ...);
	}

	template <typename Element, typename Key>
	Utility::Result<Store::Error, Element*> Component::get(Key key) const {
		return Store::Get<Element>(key, uuid);
	}

	template <typename Element>
	Utility::Result<Store::Error, Handler<Element>> Component::get(const UUID & uuid) const {
		return Store::Get<Element>(uuid);
	}

	template <typename Element>
	Utility::Result<Store::Error, Handler<Element>> Component::get() const {
		return Store::GetOne<Element>(uuid);
	}

	template<typename Element>
	Utility::Result<Store::Error, Handler<Element>> Component::getParent() const	{
		return Store::GetParent<Element>(uuid);
	}

	template<typename Element>
	Handler<Element>& Component::bind(Element * element) const {
		return Store::Bind<Element>(element, uuid);
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
