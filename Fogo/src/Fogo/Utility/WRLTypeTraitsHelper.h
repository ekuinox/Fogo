#pragma once

#include "TypeTraitsHelper.h"
#include <wrl/client.h>

namespace Fogo {
	template <typename T>
	struct is_smart_pointer<Microsoft::WRL::ComPtr<T>> : public std::true_type { using rawType = T; };

	template <typename T>
	struct is_ComPtr : public std::false_type { using rawType = T; };

	template <typename T>
	struct is_ComPtr<Microsoft::WRL::ComPtr<T>> : public std::true_type { using rawType = T; };
}