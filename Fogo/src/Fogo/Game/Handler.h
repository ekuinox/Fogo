#pragma once

#include "./UUID.h"
#include "./ContainerBase.h"
#include "./ContainerIndexKeyPair.h"
#include "./Hash.h"

namespace Fogo::Game {

	template <typename Element>
	class Handler {
	public:
		template <typename Key>
		using IndexedStore = ContainerBase<ContainerIndexKeyPair<Key>, Element*, Hash<Key>>;

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

		template <typename Key> Handler & makeIndex(const Key & key) {
			try {
				IndexedStore<Key>::shared.insert(std::make_pair(ContainerIndexKeyPair<Key> { key, parentId }, element));
			}
			catch (std::out_of_range e) {

			}
			return *this;
		}

		template <typename Key> Handler & destroyIndex(const Key & key) {
			IndexedStore<Key>::shared.erase(ContainerIndexKeyPair<Key> { key, parentId });
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
