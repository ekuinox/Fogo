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
	
	Component�ƌp���֌W�ɂ���N���XElement���Ǘ�����D
	�܂��CScene�ɑ΂��ēƎ��R���e�i��p�ӂ���D
	�擾���ɂ�Result��Ԃ��D
	UUID���ƔF�������炢�Ǝv���̂ŁCHandler�o�R�ŗv�f�ɑ΂��ĔC�ӂ̃L�[�^��p���ŃC���f�b�N�X�����邱�Ƃ��\�D
	��{�I�ɂ�Component�̃����o���Store�ɃA�N�Z�X���邱�ƂɂȂ�D

	*/


	private:
		template <typename Element>
		using Container = ContainerBase<UUID, Element*, UUID::Hash>;

		template <typename Element>
		static void Insert(Element * element, const UUID & parentId);

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
		static Handler<Element> Bind(Element * element, const UUID & parentId = rootId);

		// �|�C���^���o�C���h����
		template <typename Element, typename ElementAs, bool Both = true>
		static Handler<ElementAs> BindAs(Element * element, const UUID & parentId = rootId);

		// �\�z����
		template <typename Element, typename ... Args>
		static Handler<Element> Create(const UUID & parentId = rootId, Args ... args);

		// �\�z����
		template <typename Element, typename ElementAs, bool Both = true, typename ... Args>
		static Handler<ElementAs> CreateAs(const UUID & parentId = rootId, Args ... args);

		// UUID��p����Component���擾����
		template <typename Element>
		static Result<Error, Handler<Element>> Get(const UUID & uuid);

		// UUID��p����Component���擾����
		template <typename Element>
		static Result<Error, Handler<Element>> GetOne(const UUID & parentId);

		// �w�肵���L�[�ŊǗ����Ă���Component���擾����
		template <typename Element, typename Key>
		static Result<Error, Element*> Get(Key key, const UUID & parentId);

		// �q�𕡐��擾����
		template <typename Element>
		static std::vector<Handler<Element>> GetChildren(const UUID & parentId);

		// �w�肵��Component�̐e���擾����
		template <typename Element = Component>
		static Result<Error, Handler<Element>> GetParent(const UUID & uuid);

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
		static void Free(const UUID & parentId = rootId);

		// �R���e�i�̃T�C�Y��m��
		template <typename Element>
		static std::size_t GetSize();

		static std::size_t GetSize();

		// �q�̐����擾����
		template <typename Element = Component>
		static std::size_t GetChildrenSize(const UUID & parentId);

		// �R�s�[�𐶐�����iUUID�͍Đ�������j
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

		// �@�\������

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
