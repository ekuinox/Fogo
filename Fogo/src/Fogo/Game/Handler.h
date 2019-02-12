#pragma once

#include "./UUID.h"
#include "./ContainerBase.h"

namespace Fogo::Game {

	template <typename Element>
	class Handler {
	public:
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
				ContainerBase<Key, Element *>::shared.insert(std::make_pair(key, element));
			}
			catch (std::out_of_range e) {

			}
			return *this;
		}

		template <typename Key> Handler & destroyIndex(const Key & key) {
			ContainerBase<Key, Element *>::shared.erase(key);
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
