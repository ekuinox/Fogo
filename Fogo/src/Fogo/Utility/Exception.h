#pragma once

#include <stdexcept>
#include <Windows.h>
#include <type_traits>

namespace Fogo {
	template <typename exception = std::exception>
	void ExecOrFail(const HRESULT & hr) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (FAILED(hr)) throw exception();
	}
}
