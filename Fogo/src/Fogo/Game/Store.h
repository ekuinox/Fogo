#pragma once

#include <unordered_map>
#include <type_traits>
#include <functional>
#include "./Handler.h"
#include "./ContainerBase.h"
#include "./ComponentTree.h"
#include "./ContainerWatcherMaster.h"
#include "../Utility/Result.h"

namespace Fogo {

	class Component;
	class Scene;

	class Store final {
	
	/*
	
	Componentと継承関係にあるクラスElementを管理する．
	また，Sceneに対して独自コンテナを用意する．
	取得時にはResultを返す．
	UUIDだと認識しずらいと思うので，Handler経由で要素に対して任意のキー型を用いでインデックスをつけることも可能．
	基本的にはComponentのメンバよりStoreにアクセスすることになる．

	*/


	private:
		template <typename Element>
		using Container = ContainerBase<UUID, Element*, UUID::Hash>;

		template <typename Element>
		static void Insert(Element * element, const UUID & parentId);

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
		static Handler<Element> Bind(Element * element, const UUID & parentId = rootId);

		// ポインタをバインドする
		template <typename Element, typename ElementAs, bool Both = true>
		static Handler<ElementAs> BindAs(Element * element, const UUID & parentId = rootId);

		// 構築する
		template <typename Element, typename ... Args>
		static Handler<Element> Create(const UUID & parentId = rootId, Args ... args);

		// 構築する
		template <typename Element, typename ElementAs, bool Both = true, typename ... Args>
		static Handler<ElementAs> CreateAs(const UUID & parentId = rootId, Args ... args);

		// UUIDを用いてComponentを取得する
		template <typename Element>
		static Result<Error, Handler<Element>> Get(const UUID & uuid);

		// UUIDを用いてComponentを取得する
		template <typename Element>
		static Result<Error, Handler<Element>> GetOne(const UUID & parentId);

		// 指定したキーで管理しているComponentを取得する
		template <typename Element, typename Key>
		static Result<Error, Element*> Get(Key key, const UUID & parentId);

		// 子を複数取得する
		template <typename Element>
		static std::vector<Handler<Element>> GetChildren(const UUID & parentId);

		// 指定したComponentの親を取得する
		template <typename Element = Component>
		static Result<Error, Handler<Element>> GetParent(const UUID & uuid);

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
		static void Free(const UUID & parentId = rootId);

		// コンテナのサイズを知る
		template <typename Element>
		static std::size_t GetSize();

		static std::size_t GetSize();

		// 子の数を取得する
		template <typename Element = Component>
		static std::size_t GetChildrenSize(const UUID & parentId);

		// コピーを生成する（UUIDは再生成する）
		template <typename Element>
		static Handler<Element> Instantiate(Element & source, const UUID & parentId);

		template <typename Element>
		static Handler<Element> Instantiate(Element & source);

	};

	template <typename Element>
	void Store::Insert(Element * element, const UUID & parentId) {
		Container<Element>::shared.insert(std::make_pair(element->uuid, element));
		ComponentTree::shared->create(parentId, element->uuid);
		MakeContainerMaster<Container<Element>>::Entry(element->uuid);
	}

	template <typename Element>
	constexpr bool Store::IsCorrectElement() {
		return std::is_base_of<Component, Element>();
	}

	template <typename Element>
	Handler<Element> Store::Bind(Element * element, const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());

		Insert<Element>(element, parentId);

		Insert<Component>(element, parentId);

		if constexpr (std::is_base_of<Scene, Element>()) Insert<Scene>(element, parentId);

		return Handler<Element>	(element);
	}

	template<typename Element, typename ElementAs, bool Both>
	Handler<ElementAs> Store::BindAs(Element * element, const UUID & parentId) {
		static_assert(std::is_base_of<ElementAs, Element>());
		if constexpr (Both) {
			Bind<Element>(element, parentId);
		}
		Insert<ElementAs>(element, parentId);
		return Container<ElementAs>::shared.at(element->uuid);
	}

	template <typename Element, typename ... Args>
	Handler<Element> Store::Create(const UUID & parentId, Args ... args) {
		return Bind(new Element(args...), parentId);
	}

	template<typename Element, typename ElementAs, bool Both, typename ...Args>
	Handler<ElementAs> Store::CreateAs(const UUID & parentId, Args ...args) {
		static_assert(std::is_base_of<ElementAs, Element>());
		return BindAs<Element, ElementAs, Both>(new Element(args...), parentId);
	}

	template <typename Element>
	Result<Store::Error, Handler<Element>> Store::Get(const UUID & uuid) {
		static_assert(IsCorrectElement<Element>());
		try {
			return Container<Element>::shared.at(uuid);
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template <typename Element>
	Result<Store::Error, Handler<Element>> Store::GetOne(const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());

		std::vector<UUID> uuids {};

		for (const auto & child : ComponentTree::shared->getChildren(parentId)) {
			if (Container<Element>::shared.count(child) != 0) {
				uuids.emplace_back(child);
			}
 		}

		if (uuids.size() == 1) {
			return Container<Element>::shared.at(uuids.front());
		}

		return Error::ChildrenNotOne;
	}

	template <typename Element, typename Key>
	Result<Store::Error, Element*> Store::Get(Key key, const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());
		try {
			return ContainerBase<ContainerIndexKeyPair<Key>, Element*, Hash<Key>>::shared.at(ContainerIndexKeyPair<Key> { key, parentId });
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template<typename Element>
	std::vector<Handler<Element>> Store::GetChildren(const UUID& parentId)
	{
		std::vector<Handler<Element>> result{};
		for (const auto& [_, element] : Container<Element>::shared) {
			if (element.getParentUUID() == parentId) result.emplace_back(element);
		}
		return result;
	}

	template <typename Element>
	Result<Store::Error, Handler<Element>> Store::GetParent(const UUID & uuid) {
		static_assert(IsCorrectElement<Element>());
		try {
			if (const auto & parentId = ComponentTree::shared->getParent(uuid)) {
				if (*parentId == rootId) {
					return Error::YourParentIsRoot;
				}
				return Handler<Element>(Container<Element>::shared.at(*parentId));
			}
			return Error::NotExist;
		}
		catch (std::out_of_range e) {
			return Error::NotExist;
		}
	}

	template <typename Element>
	void Store::Execute(const std::function<void(Element &)> & func) {
		static_assert(IsCorrectElement<Element>());
		for (const auto &[_, element] : Container<Element>::shared) {
			func(*element);
		}
	}

	template <typename Element>
	void Store::Execute(const std::function<void(Element &)> & func, const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());
		for (const auto & child : ComponentTree::shared->getChildren(parentId)) {
			try {
				func(*Container<Element>::shared.at(child));
			} catch (...) {}
		}
	}

	template <typename Element>
	Element * Store::Detach(const UUID & uuid) {
		static_assert(IsCorrectElement<Element>());

		// 機能未実装

		/*
		const auto element = Container<Element>::shared.at(uuid).element;

		Container<Element>::shared.erase(uuid);

		Container<Component>::shared.erase(uuid);

		if constexpr (std::is_base_of<Scene, Element>()) {
			Container<Scene>::shared.erase(uuid);
		}
		
		return element;
		*/
		return nullptr;
	}

	template <typename Element>
	std::size_t Store::GetSize() {
		return Container<Element>::shared.size();
	}

	template <typename Element>
	std::size_t Store::GetChildrenSize(const UUID & parentId) {
		static_assert(IsCorrectElement<Element>());

		const auto & children = ComponentTree::shared->getChildren(parentId);

		if constexpr (std::is_same<Component, Element>()) return children.size();

		std::size_t size = 0;
		for (const auto & child : children) {
			if (Container<Element>::shared.count(child) != 0) size += 1;
		}

		return size;
	}

	template <typename Element>
	Handler<Element> Store::Instantiate(Element & source, const UUID & parentId) {
		static_assert(std::is_base_of<Component, Element>());

		auto clone = new Element;

		std::memcpy(clone, &source, sizeof Element);

		const_cast<UUID::Engine::result_type&>(clone->uuid.raw) = UUID::Generate();

		return Bind(clone, parentId);
	}

	template <typename Element>
	Handler<Element> Store::Instantiate(Element & source) {
		return Instantiate(source, ComponentTree::shared->getParent(source.uuid));
	}

}
