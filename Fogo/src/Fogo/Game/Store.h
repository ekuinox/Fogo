#pragma once

#include <unordered_map>
#include <type_traits>
#include <functional>
#include "./Handler.h"
#include "./ContainerBase.h"
#include "../Utility/Result.h"

namespace Fogo::Game {

	class Component;
	class Scene;

	class Store final {
	
	/*
	
	Componentと継承関係にあるクラスElementを管理する．
	また，Sceneに対して独自コンテナを用意する．
	呼び出し時には，Elementの型やRenderableなどのInterfaceの役割を担うクラスをテンプレート引数にて指定し，uuidを持って取得する．
	取得時にはResultを返す．
	UUIDだと認識しずらいと思うので，Handler経由で要素に対して任意のキー型を用いでインデックスをつけることも可能．
	基本的にはComponentのメンバよりStoreにアクセスすることになる．

	*/


	private:
		template <typename Element>
		using Container = ContainerBase<UUID, Handler<Element>, UUID::Hash>;

		template <typename Element>
		static void Insert(Element * element, const UUID & parentId);

		template <typename Element>
		static void Insert(Element * element, const UUID & parentId, const UUID & uuid);

	public:
		enum class Error {
			NotExist, // Elementが取得できなかったときに投げる
			YourParentIsRoot, // rootがparentを検索したときに投げる
			ChildrenNotOne, // GetOne時に個数が1じゃなかった場合
		};

		static const UUID rootId;

		// 要件を満たした型か確認する
		template <typename Element>
		static constexpr bool IsCorrectElement();

		// ポインタをバインドする
		template <typename Element>
		static Handler<Element> & Bind(Element * element, const UUID & parentId = rootId);

		// ポインタをバインドする
		template <typename Element, typename ElementAs, bool Both = true>
		static Handler<ElementAs> & BindAs(Element * element, const UUID & parentId = rootId);

		// 構築する
		template <typename Element, typename ... Args>
		static Handler<Element> & Create(const UUID & parentId = rootId, Args ... args);

		// 構築する
		template <typename Element, typename ElementAs, bool Both = true, typename ... Args>
		static Handler<ElementAs> & CreateAs(const UUID & parentId = rootId, Args ... args);

		// UUIDを用いてComponentを取得する
		template <typename Element>
		static Utility::Result<Error, Handler<Element>> Get(const UUID & uuid);

		// UUIDを用いてComponentを取得する
		template <typename Element>
		static Utility::Result<Error, Handler<Element>> GetOne(const UUID & parentId);

		// 指定したキーで管理しているComponentを取得する
		template <typename Element, typename Key>
		static Utility::Result<Error, Element*> Get(Key key, const UUID & parentId);

		// 指定したComponentの親を取得する
		template <typename Element = Component>
		static Utility::Result<Error, Handler<Element>> GetParent(const UUID & uuid);

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

		// 子の数を取得する
		template <typename Element = Component>
		static std::size_t GetChildrenSize(const UUID & parentId);

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
		return std::is_base_of<Component, Element>();
	}

	template <typename Element>
	Handler<Element> & Store::Bind(Element * element, const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());

		Insert<Element>(element, parentId);

		Insert<Component>(element, parentId);

		if constexpr (std::is_base_of<Scene, Element>()) Insert<Scene>(element, parentId);

		return Container<Element>::shared.at(element->uuid);
	}

	template<typename Element, typename ElementAs, bool Both>
	Handler<ElementAs> & Store::BindAs(Element * element, const UUID & parentId) {
		static_assert(std::is_base_of<ElementAs, Element>());
		if constexpr (Both) {
			Bind<Element>(element, parentId);
		}
		Insert<ElementAs>(element, parentId);
	}

	template <typename Element, typename ... Args>
	Handler<Element> & Store::Create(const UUID & parentId, Args ... args) {
		return Bind(new Element(args...), parentId);
	}

	template<typename Element, typename ElementAs, bool Both, typename ...Args>
	Handler<ElementAs>& Store::CreateAs(const UUID & parentId, Args ...args) {
		static_assert(std::is_base_of<ElementAs, Element>());
		return BindAs<Element, ElementAs, Both>(new Element(args...), parentId);
	}

	template <typename Element>
	Utility::Result<Store::Error, Handler<Element>> Store::Get(const UUID & uuid) {
		static_assert(IsCorrectElement<Element>());
		try {
			return Container<Element>::shared.at(uuid);
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template <typename Element>
	Utility::Result<Store::Error, Handler<Element>> Store::GetOne(const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());

		std::vector<UUID> uuids {};

		for (const auto &[uuid, element] : Container<Element>::shared) {
			if (element.parentId == parentId) uuids.emplace_back(uuid);
		}

		if (uuids.size() == 1) {
			return Container<Element>::shared.at(uuids.front());
		}

		return Error::ChildrenNotOne;
	}

	template <typename Element, typename Key>
	Utility::Result<Store::Error, Element*> Store::Get(Key key, const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());
		try {
			auto & container = ContainerBase<ContainerIndexKeyPair<Key>, Element*, Hash<Key>>::shared;
			return container.at(ContainerIndexKeyPair<Key> { key, parentId });
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template <typename Element>
	Utility::Result<Store::Error, Handler<Element>> Store::GetParent(const UUID & uuid) {
		static_assert(IsCorrectElement<Element>());
		try {
			auto parentId = Container<Component>::shared.at(uuid).parentId;
			if (parentId == rootId) {
				return Error::YourParentIsRoot;
			}
			return Container<Element>::shared.at(parentId);
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template <typename Element>
	void Store::Execute(const std::function<void(Element &)> & func) {
		static_assert(IsCorrectElement<Element>());
		for (const auto &[_, element] : Container<Element>::shared) {
			func(*(element.element));
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

		if constexpr (std::is_base_of<Scene, Element>()) {
			Container<Scene>::shared.erase(uuid);
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
			// 最後にComponentが回ってくるのでそこでdelete
			if constexpr (std::is_same<Component, Element>()) {
				delete Container<Element>::shared[uuid].element;
				Container<Element>::shared[uuid].element = nullptr;
			}

			Container<Element>::shared.erase(uuid);
		}

		if constexpr (sizeof...(Elements) > 0) {
			Free<Elements...>(parentId);
		}
	}

	template <typename Element>
	std::size_t Store::GetSize() {
		return Container<Element>::shared.size();
	}

	template <typename Element>
	std::size_t Store::GetChildrenSize(const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());
		std::size_t size = 0;
		for (const auto &[_, element] : Container<Element>::shared) {
			if (element.parentId == parentId) ++size;
		}
		return size;
	}
}
