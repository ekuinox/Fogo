#pragma once

#include <type_traits>

namespace Fogo::TypeTraitsHelper
{
	template <typename T1, typename T2, typename ... Pairs>
	constexpr bool hasContained()
	{
		if constexpr (std::is_same<T1, T2>()) return true;
		if constexpr (sizeof...(Pairs) > 0) return hasContained<T1, Pairs...>();
		return false;
	}

	template <typename T1, typename T2>
	constexpr bool isDerivedPointerTraits()
	{
		return std::is_base_of<std::pointer_traits<T1>::element_type, std::pointer_traits<T2>::element_type>();
	}

	template <typename Base, typename Derived, typename ... Types>
	constexpr bool isDerived()
	{
		if constexpr (isDerivedPointerTraits<Base, Derived>()) return true;
		if constexpr (sizeof...(Types) > 0) return isDerived<Base, Types...>();
		return false;
	}

	template <typename T>
	struct is_smart_pointer : public std::false_type { using rawType = T; };

	template <typename T>
	struct is_smart_pointer<std::unique_ptr<T>> : public std::true_type { using rawType = T; };

	template <typename T>
	struct is_smart_pointer<std::shared_ptr<T>> : public std::true_type { using rawType = T; };

	template <typename T>
	struct is_smart_pointer<std::weak_ptr<T>> : public std::true_type { using rawType = T; };

	// shared_ptrのチェック
	template <typename T>
	struct is_shared_ptr : public std::false_type { using rawType = T; };

	template <typename T>
	struct is_shared_ptr<std::shared_ptr<T>> : public std::true_type { using rawType = T; };

	// unique_ptrのチェック
	template <typename T>
	struct is_unique_ptr : public std::false_type { using rawType = T; };

	template <typename T>
	struct is_unique_ptr<std::unique_ptr<T>> : public std::true_type {	using rawType = T; };

	// weak_ptrのチェック
	template <typename T>
	struct is_weak_ptr : public std::false_type { using rawType = T; };

	template <typename T>
	struct is_weak_ptr<std::weak_ptr<T>> : public std::true_type { using rawType = T; };
}
