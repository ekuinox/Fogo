#pragma once

#include <stdexcept>
#include <Windows.h>
#include <type_traits>

namespace Fogo::Utility {
	template <typename exception = std::exception>
	void ExecOrFail(const HRESULT & hr) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (FAILED(hr)) throw exception();
	}
	template <typename exception = std::exception>
	void ExecOrFail(const HANDLE & handle) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (handle == nullptr) throw exception();
	}
	template <typename exception = std::exception>
	void ExecOrFail(const bool & condition) {
		static_assert(std::is_base_of<std::exception, exception>());
		if (!condition) throw exception();
	}
}
