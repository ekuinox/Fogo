#pragma once

#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include "TypeTraitsHelper.h"

/*
* author: Hibiki Kominami <github.com/lm9>
* 継承関係のあるクラスをまとめて管理するためのクラス
* 特化されたものにキャストなしでアクセスできるようにしている
* アップキャストは発生する
* 現状std::pointer_traitsな型しか通せない
* 現状、Elmの型さえ違えばKeyの重複が可能になっている
* Baseにvoidを指定すると継承関係を無視できる
*/

namespace Fogo {

	template <class Base, typename Key, typename ... Elms>
	class TreeStore
	{
	public:
		// オブジェクトの親子関係を表現するための構造体
		template <typename Elm>
		struct Holder
		{
			Key key, parentKey;
			Elm element;
		};
	
		// 再帰的に実行するラムダのエイリアス
		template <class T> using ExecuteFunction = std::function<void(T)>;
	
	private:
		// コンストラクタ必要なしっぽい
		TreeStore(const TreeStore &) = delete;
		TreeStore() = delete;
	
		// 実際のオブジェクトを管理する静的なunordered_map
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
	
		// 指定した型が含まれているかチェックする
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
	
		// 指定した型がBaseを継承しているか静的にチェックする2
		// Baseがvoidの場合継承関係を無視するし、管理する型がpointer_traitsでなくとも通す
		template <typename Elm>
		static constexpr bool __isDerived()
		{
			if constexpr (std::is_void<Base>()) return true;
			else return __isDerived<Base, Elm>();
		}
	
		// 指定した型がBaseを継承しているか静的にチェックする3
		template <typename Elm>
		static constexpr void __staticAssertDerivedCheck()
		{
			static_assert(__isDerived<Elm>(), "指定された型がBaseを継承していません");
		}
	
		// Reserveのヘルパ
		template <typename Elm, typename ... Elms>
		static void __reserve(std::size_t size)
		{
			__staticAssertDerivedCheck<Elm>();
			Container<Elm>::GetInstance().reserve(size);
			if constexpr (sizeof...(Elms) > 0) __reserve<Elms...>(size);
		}
	
		// テンプレートで指定した型が条件を満たしているかチェックする
		template <typename Elm>
		static void __precheck()
		{
			__staticAssertDerivedCheck<Elm>();
			static_assert(__hasContained<Elm, Elms...>(), "指定された型はこのストアで管理されていません");
		}
	
		// 指定した基底型を使って再帰的に実行するヘルパ
		// Baseの子と孫クラスを管理する際に子クラスで回したい場合に
		template <typename Base, typename Elm, typename ... Elms>
		static void __execute(ExecuteFunction<Base> func)
		{
			if constexpr (__isDerived<Base, Elm>())
				for (const auto & [key, holder] : Container<Elm>::GetInstance())
					func(holder.element);
			if constexpr (sizeof...(Elms) > 0) __execute<Base, Elms...>(func);
		}
	
		// __executeを親ID指定で実行する
		template <typename Base, typename Elm, typename ... Elms>
		static void __execute(ExecuteFunction<Base> func, Key parentKey)
		{
			if constexpr (__isDerived<Base, Elm>())
				for (const auto &[key, holder] : Container<Elm>::GetInstance())
					if (holder.parentKey == parentKey) func(holder.element);
			if constexpr (sizeof...(Elms) > 0) __execute<Base, Elms...>(func, parentKey);
		}
	
	public:
		// 事前に領域を確保する（これを呼ばずにInsertしてもとりあえずは問題なく動く）
		static void Reserve(std::size_t size = 0)
		{
			__reserve<Elms...>(size);
		}
	
		// 指定したキーからオブジェクトを取得する
		template <typename Elm>
		static const Elm & Get(Key key)
		{
			__precheck<Elm>();
			return Container<Elm>::GetInstance().at(key).element;
		}
	
		// 指定したキーに対してオブジェクトを挿入する
		template <typename Elm>
		static bool Insert(Key key, Key parentKey, Elm elm)
		{
			__precheck<Elm>();
			Container<Elm>::GetInstance()[key] = Holder<Elm>{
				key, parentKey, elm
			};
			return true;
		}
	
		// 指定された基底クラスで再帰的に実行する
		template <typename Elm = Base>
		static void Execute(ExecuteFunction<Elm> func)
		{
			static_assert(TypeTraitsHelper::isDerived<Elm, Elms...>());
			__execute<Elm, Elms...>(func);
		}
	
		// 指定された基底クラスを親Keyを指定して実行する
		template <typename Elm = Base>
		static void Execute(ExecuteFunction<Elm> func, Key parentKey)
		{
			static_assert(TypeTraitsHelper::isDerived<Elm, Elms...>());
			__execute<Elm, Elms...>(func, parentKey);
		}
	};

} // namespace Fogo
