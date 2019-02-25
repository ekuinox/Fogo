#pragma once

#include <functional>
#include "./Handler.h"
#include "./Store.h"

namespace Fogo::Game {

	class Component {
	/*

	Component���ׂĂ̊��N���X
	UUID��Store�̃��b�p�@�\�̂�

	*/
	public:
		const UUID uuid;

		Component();
		virtual ~Component();
		Component(const Component &) = delete;
		Component & operator=(const Component &) = delete;

		// uuid��p���Ĕ�r����
		bool operator==(const Component & rhs) const;

		// �����_�����g���Ď��s����
		template <typename Element>
		void execute(const std::function<void(Element &)> func) const;

		// �C���f�b�N�X�p�L�[��p���Ďq�C���X�^���X���擾����
		template <typename Element, typename Key>
		Utility::Result<Store::Error, Element*> get(Key key) const;

		// UUID��p���Ďq�C���X�^���X�̃n���h�����擾����
		template <typename Element>
		Utility::Result<Store::Error, Handler<Element>> get(const UUID & uuid) const;

		// ��̎q�C���X�^���X�̃n���h�����擾����
		// �P��ł��邱�Ƃ�ۏ؂��邽�߂ɁA0�������̏ꍇ�̓G���[���Ԃ�
		template <typename Element>
		Utility::Result<Store::Error, Handler<Element>> get() const;

		// �����������Ă���Scene���擾����
		Utility::Result<Store::Error, Handler<Scene>> getMyScene() const;
		
		// �e�C���X�^���X�̃n���h�����擾������
		template <typename Element = Component>
		Utility::Result<Store::Error, Handler<Element>> getParent() const;

		// �q�C���X�^���X�𐶐�����
		template <typename Element, typename ... Args>
		Handler<Element> & create(Args ... args) const;

		// �q�C���X�^���X�Ƃ��ăo�C���h����
		template <typename Element>
		Handler<Element> & bind(Element * element) const;

		// �q�C���X�^���X�����ׂĉ������
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
