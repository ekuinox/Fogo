#pragma once

#include <unordered_map>
#include <type_traits>
#include "./Handler.h"
#include "./ContainerBase.h"

class Store final {
private:
	template <typename Element>
	using Container = ContainerBase<UUID, Handler<Element>, UUID::Hash>;

	template <typename Element>
	static void Insert(Element * element, const UUID & parentId);
	
	template <typename Element>
	static void Insert(Element * element, const UUID & parentId, const UUID & uuid);

public:
	static const UUID rootId;

	// �v���𖞂������^���m�F����
	template <typename Element>
	static constexpr bool IsCorrectElement();

	// �|�C���^���o�C���h����
	template <typename Element>
	static Handler<Element> & Bind(Element * element, const UUID & parentId = rootId);

	// �\�z����
	template <typename Element, typename ... Args>
	static Handler<Element> & Create(const UUID & parentId = rootId, Args ... args);

	// UUID��p����Component���擾����
	template <typename Element>
	static Handler<Element> & Get(const UUID & uuid);

	// �w�肵���L�[�ŊǗ����Ă���Component���擾����
	template <typename Element, typename Key>
	static Element & Get(const Key & key);
	
	// �w�肵��Component�̐e���擾����
	template <typename Element = Component>
	static Handler<Element> & GetParent(const UUID & uuid);

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
	static const std::size_t GetSize();

	static const std::size_t GetSize();

};
