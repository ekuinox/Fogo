#pragma once

#include "./UUID.h"
#include "./ContainerBase.h"
#include "./ContainerIndexKeyPair.h"
#include "./Hash.h"
#include "ContainerWatcherMaster.h"

namespace Fogo {

	template <typename Element>
	class Handler {
	private:
		template <typename Key, typename Elm = Element>
		using IndexedStore = ContainerBase<ContainerIndexKeyPair<Key>, Elm*, Hash<Key>>;
		
		Element * element;
		UUID parentId;

	public:
		Handler(Element * element, const UUID & parentId) : element(element), parentId(parentId) {

		}

		Handler() : Handler(nullptr, UUID()) {
			// �Ȃ�ł���񂱂�
		}

		static Handler Create(Element * element, const UUID & parentId) {
			return std::move(Handler(element, parentId));
		}

		// �p�����N���X�ł̎擾
		template <typename ElementAs>
		Handler<ElementAs> as() const {
			static_assert(std::is_base_of<ElementAs, Element>());
			if (const auto & elementAs = Store::Get<ElementAs>(element->uuid)) {
				return *elementAs;
			}
			// �i�[����Ă��Ȃ��ꍇ�V���Ɋi�[���ĕԋp
			return Store::BindAs<Element, ElementAs, false>(element, parentId);
		}

		// �C���f�b�N�X�̍쐬
		template <typename Elm = Element, typename Key>
		Handler & makeIndex(Key key) {
			IndexedStore<Key, Elm>::shared.insert(std::make_pair(ContainerIndexKeyPair<Key> { key, parentId }, element));
			ContainerWatcherMaster::shared->create<IndexedStore<Key, Elm>>();
			return *this;
		}

		// �C���f�b�N�X�̍폜
		template <typename Elm = Element, typename Key>
		Handler & destroyIndex(Key key) {
			IndexedStore<Key, Elm>::shared.erase(ContainerIndexKeyPair<Key> { key, parentId });
			return *this;
		}

		// �e�̑}���ւ�
		Handler & assign(const UUID & newParentId) {
			parentId = newParentId;
			return *this;
		}

		// element�ւ̃����o�A�N�Z�X
		Element * operator->() const {
			return element;
		}

		// element�ւ̎Q�Ƃ𓾂�
		Element & operator*() const {
			return *element;
		}

		// element�̃|�C���^�𓾂�
		Element * operator&() const {
			return element;
		}

		// �eUUID�𓾂�
		const UUID getParentUUID() const {
			return parentId;
		}

		// �������
		void release() {
			delete element;
			element = nullptr;
		}
	};

	// element��nullptr���`�F�b�N����
	template <typename Element>
	struct HandlerChecker {
		bool operator()(const Handler<Element> & handler) const {
			return &handler != nullptr;
		}
	};

}
