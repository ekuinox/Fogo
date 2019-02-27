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
	
	Component�ƌp���֌W�ɂ���N���XElement���Ǘ�����D
	�܂��CScene�ɑ΂��ēƎ��R���e�i��p�ӂ���D
	�Ăяo�����ɂ́CElement�̌^��Renderable�Ȃǂ�Interface�̖�����S���N���X���e���v���[�g�����ɂĎw�肵�Cuuid�������Ď擾����D
	�擾���ɂ�Result��Ԃ��D
	UUID���ƔF�������炢�Ǝv���̂ŁCHandler�o�R�ŗv�f�ɑ΂��ĔC�ӂ̃L�[�^��p���ŃC���f�b�N�X�����邱�Ƃ��\�D
	��{�I�ɂ�Component�̃����o���Store�ɃA�N�Z�X���邱�ƂɂȂ�D

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
			NotExist, // Element���擾�ł��Ȃ������Ƃ��ɓ�����
			YourParentIsRoot, // root��parent�����������Ƃ��ɓ�����
			ChildrenNotOne, // GetOne���Ɍ���1����Ȃ������ꍇ
		};

		static const UUID rootId;

		// �v���𖞂������^���m�F����
		template <typename Element>
		static constexpr bool IsCorrectElement();

		// �|�C���^���o�C���h����
		template <typename Element>
		static Handler<Element> & Bind(Element * element, const UUID & parentId = rootId);

		// �|�C���^���o�C���h����
		template <typename Element, typename ElementAs, bool Both = true>
		static Handler<ElementAs> & BindAs(Element * element, const UUID & parentId = rootId);

		// �\�z����
		template <typename Element, typename ... Args>
		static Handler<Element> & Create(const UUID & parentId = rootId, Args ... args);

		// �\�z����
		template <typename Element, typename ElementAs, bool Both = true, typename ... Args>
		static Handler<ElementAs> & CreateAs(const UUID & parentId = rootId, Args ... args);

		// UUID��p����Component���擾����
		template <typename Element>
		static Utility::Result<Error, Handler<Element>> Get(const UUID & uuid);

		// UUID��p����Component���擾����
		template <typename Element>
		static Utility::Result<Error, Handler<Element>> GetOne(const UUID & parentId);

		// �w�肵���L�[�ŊǗ����Ă���Component���擾����
		template <typename Element, typename Key>
		static Utility::Result<Error, Element*> Get(Key key, const UUID & parentId);

		// �w�肵��Component�̐e���擾����
		template <typename Element = Component>
		static Utility::Result<Error, Handler<Element>> GetParent(const UUID & uuid);

		// �R���e�i�̗v�f�ɑ΂��Ď��s����
		template <typename Element>
		static void Execute(const std::function<void(Element &)> & func);

		// �R���e�i�̔C�ӂ̐eID���������v�f�ɑ΂��Ď��s����
		template <typename Element>
		static void Execute(const std::function<void(Element &)> & func, const UUID & parentId);

		// �Ǘ����������
		template <typename Element>
		static Element * Detach(const UUID & uuid);

		// �R���e�i�̒��g���J�����Ă���
		template <typename Element, typename ... Elements>
		static void Free(const UUID & parentId = rootId);

		static void Free(const UUID & parentId = rootId);

		// �R���e�i�̃T�C�Y��m��
		template <typename Element>
		static std::size_t GetSize();

		static std::size_t GetSize();

		// �q�̐����擾����
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
			// �Ō��Component������Ă���̂ł�����delete
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
