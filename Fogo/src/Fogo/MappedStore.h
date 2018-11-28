#pragma once

#include <unordered_map>
#include <stdexcept>
#include <functional>

/*
* author: Hibiki Kominami <github.com/lm9>
* 継承関係のないオブジェクトを管理するためのクラス
*/

namespace Fogo {

	template <typename ... Pairs>
	class MappedStore
	{
	private:
		template<typename Key, typename Elm>
		class Container : public std::unordered_map<Key, Elm>
		{
		private:
			Container() : std::unordered_map<Key, Elm>({}) {}
		public:
			static Container & GetInstance()
			{
				static Container instance;
				return instance;
			}
		};
	
		template <typename Key1, typename Elm1, typename Key2, typename Elm2, typename ... Pairs>
		static constexpr bool hasContainedPair()
		{
			if constexpr (std::is_same<Key1, Key2>() && std::is_same<Elm1, Elm2>()) return true;
			if constexpr (sizeof...(Pairs) >= 2) return hasContainedPair<Key1, Elm1, Pairs...>();
			return false;
		}
	
		template <typename T1, typename T2, typename ... Pairs>
		static constexpr bool hasContained()
		{
			if constexpr (std::is_same<T1, T2>()) return true;
			if constexpr (sizeof...(Pairs) > 0) return hasContained<T1, Pairs...>();
			return false;
		}
	
		template <typename Key, typename Elm, typename ... Pairs>
		static void __reserve(std::size_t size)
		{
			Container<Key, Elm>::GetInstance().reserve(size);
			if constexpr (sizeof...(Pairs) > 0)
			{
				__reserve<Pairs...>(size);
			}
		}
	
		MappedStore(const MappedStore &) = delete;
		MappedStore() = delete;
	
	public:
		static void Reserve(std::size_t size = 0)
		{
			__reserve<Pairs...>(size);
		}
		
		template <typename Elm, typename Key>
		static const Elm & Get(Key key)
		{
			static_assert(hasContainedPair<Key, Elm, Pairs ...>());
			return Container<Key, Elm>::GetInstance().at(key);
		}
	
		template <typename Key, typename Elm>
		static void Insert(Key key, Elm elm)
		{
			static_assert(hasContainedPair<Key, Elm, Pairs ...>());
			Container<Key, Elm>::GetInstance()[key] = elm;
		}
	
	};

} // namespace Fogo
