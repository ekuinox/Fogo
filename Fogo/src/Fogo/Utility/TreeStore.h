#pragma once

#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include "TypeTraitsHelper.h"

/*
* author: Hibiki Kominami <github.com/lm9>
* �p���֌W�̂���N���X���܂Ƃ߂ĊǗ����邽�߂̃N���X
* �������ꂽ���̂ɃL���X�g�Ȃ��ŃA�N�Z�X�ł���悤�ɂ��Ă���
* �A�b�v�L���X�g�͔�������
* ����std::pointer_traits�Ȍ^�����ʂ��Ȃ�
* ����AElm�̌^�����Ⴆ��Key�̏d�����\�ɂȂ��Ă���
* Base��void���w�肷��ƌp���֌W�𖳎��ł���
*/

namespace Fogo {

	template <class Base, typename Key, typename ... Elms>
	class TreeStore
	{
	public:
		// �I�u�W�F�N�g�̐e�q�֌W��\�����邽�߂̍\����
		template <typename Elm>
		struct Holder
		{
			Key key, parentKey;
			Elm element;
		};
	
		// �ċA�I�Ɏ��s���郉���_�̃G�C���A�X
		template <class T> using ExecuteFunction = std::function<void(T)>;
	
	private:
		// �R���X�g���N�^�K�v�Ȃ����ۂ�
		TreeStore(const TreeStore &) = delete;
		TreeStore() = delete;
	
		// ���ۂ̃I�u�W�F�N�g���Ǘ�����ÓI��unordered_map
		template<typename Elm>
		class Container : public std::unordered_map<Key, Holder<Elm>>
		{
		private:
			Container() : std::unordered_map<Key, Holder<Elm>>({}) {}
		public:
			static Container & GetInstance()
			{
				static Container instance;
				return instance;
			}
		};
	
		// �w�肵���^���܂܂�Ă��邩�`�F�b�N����
		template <typename T1, typename T2, typename ... Elms>
		static constexpr bool __hasContained()
		{
			if constexpr (std::is_same<T1, T2>()) return true;
			if constexpr (sizeof...(Elms) > 0) return __hasContained<T1, Elms...>();
			return false;
		}
	
		template <typename Base, typename T>
		static constexpr bool __isDerived()
		{
			return std::is_base_of<std::pointer_traits<Base>::element_type, std::pointer_traits<T>::element_type>();
		}
	
		// �w�肵���^��Base���p�����Ă��邩�ÓI�Ƀ`�F�b�N����2
		// Base��void�̏ꍇ�p���֌W�𖳎����邵�A�Ǘ�����^��pointer_traits�łȂ��Ƃ��ʂ�
		template <typename Elm>
		static constexpr bool __isDerived()
		{
			if constexpr (std::is_void<Base>()) return true;
			else return __isDerived<Base, Elm>();
		}
	
		// �w�肵���^��Base���p�����Ă��邩�ÓI�Ƀ`�F�b�N����3
		template <typename Elm>
		static constexpr void __staticAssertDerivedCheck()
		{
			static_assert(__isDerived<Elm>(), "�w�肳�ꂽ�^��Base���p�����Ă��܂���");
		}
	
		// Reserve�̃w���p
		template <typename Elm, typename ... Elms>
		static void __reserve(std::size_t size)
		{
			__staticAssertDerivedCheck<Elm>();
			Container<Elm>::GetInstance().reserve(size);
			if constexpr (sizeof...(Elms) > 0) __reserve<Elms...>(size);
		}
	
		// �e���v���[�g�Ŏw�肵���^�������𖞂����Ă��邩�`�F�b�N����
		template <typename Elm>
		static void __precheck()
		{
			__staticAssertDerivedCheck<Elm>();
			static_assert(__hasContained<Elm, Elms...>(), "�w�肳�ꂽ�^�͂��̃X�g�A�ŊǗ�����Ă��܂���");
		}
	
		// �w�肵�����^���g���čċA�I�Ɏ��s����w���p
		// Base�̎q�Ƒ��N���X���Ǘ�����ۂɎq�N���X�ŉ񂵂����ꍇ��
		template <typename Base, typename Elm, typename ... Elms>
		static void __execute(ExecuteFunction<Base> func)
		{
			if constexpr (__isDerived<Base, Elm>())
				for (const auto & [key, holder] : Container<Elm>::GetInstance())
					func(holder.element);
			if constexpr (sizeof...(Elms) > 0) __execute<Base, Elms...>(func);
		}
	
		// __execute��eID�w��Ŏ��s����
		template <typename Base, typename Elm, typename ... Elms>
		static void __execute(ExecuteFunction<Base> func, Key parentKey)
		{
			if constexpr (__isDerived<Base, Elm>())
				for (const auto &[key, holder] : Container<Elm>::GetInstance())
					if (holder.parentKey == parentKey) func(holder.element);
			if constexpr (sizeof...(Elms) > 0) __execute<Base, Elms...>(func, parentKey);
		}
	
	public:
		// ���O�ɗ̈���m�ۂ���i������Ă΂���Insert���Ă��Ƃ肠�����͖��Ȃ������j
		static void Reserve(std::size_t size = 0)
		{
			__reserve<Elms...>(size);
		}
	
		// �w�肵���L�[����I�u�W�F�N�g���擾����
		template <typename Elm>
		static const Elm & Get(Key key)
		{
			__precheck<Elm>();
			return Container<Elm>::GetInstance().at(key).element;
		}
	
		// �w�肵���L�[�ɑ΂��ăI�u�W�F�N�g��}������
		template <typename Elm>
		static bool Insert(Key key, Key parentKey, Elm elm)
		{
			__precheck<Elm>();
			Container<Elm>::GetInstance()[key] = Holder<Elm>{
				key, parentKey, elm
			};
			return true;
		}
	
		// �w�肳�ꂽ���N���X�ōċA�I�Ɏ��s����
		template <typename Elm = Base>
		static void Execute(ExecuteFunction<Elm> func)
		{
			static_assert(TypeTraitsHelper::isDerived<Elm, Elms...>());
			__execute<Elm, Elms...>(func);
		}
	
		// �w�肳�ꂽ���N���X��eKey���w�肵�Ď��s����
		template <typename Elm = Base>
		static void Execute(ExecuteFunction<Elm> func, Key parentKey)
		{
			static_assert(TypeTraitsHelper::isDerived<Elm, Elms...>());
			__execute<Elm, Elms...>(func, parentKey);
		}
	};

} // namespace Fogo
