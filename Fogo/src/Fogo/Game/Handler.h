#pragma once

#include "./UUID.h"
#include "./ContainerBase.h"
#include "./ContainerIndexKeyPair.h"
#include "./Hash.h"

namespace Fogo::Game {

	template <typename Element>
	class Handler {
	public:
		template <typename Key, typename Elm = Element>
		using IndexedStore = ContainerBase<ContainerIndexKeyPair<Key>, Elm*, Hash<Key>>;

		Element * element;
		UUID parentId;

		Handler(Element * element, const UUID & parentId) : element(element), parentId(parentId) {

		}

		Handler() : Handler(nullptr, UUID()) {
			// ‚È‚ñ‚Å‚¢‚é‚ñ‚±‚ê
		}

		static Handler Create(Element * element, const UUID & parentId) {
			return std::move(Handler(element, parentId));
		}

		template <typename Elm = Element, typename Key>
		Handler & makeIndex(Key key) {
			IndexedStore<Key, Elm>::shared.insert(std::make_pair(ContainerIndexKeyPair<Key> { key, parentId }, element));
			return *this;
		}

		template <typename Elm = Element, typename Key>
		Handler & destroyIndex(Key key) {
			IndexedStore<Key, Elm>::shared.erase(ContainerIndexKeyPair<Key> { key, parentId });
			return *this;
		}

		Handler & assign(const UUID & newParentId) {
			parentId = newParentId;
			return *this;
		}

		Element * operator->() const {
			return element;
		}

		Element & operator*() const {
			return *element;
		}
	};

}
