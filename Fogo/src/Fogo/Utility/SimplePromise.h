#pragma once

#include <Windows.h>
#include <functional>

namespace Fogo::Utility {
	class SimplePromise {
		std::function<void(void)> __onSuccess = [] {};
		std::function<void(void)> __onFailure = [] {};
		std::function<HRESULT(void)> __task = [] { return S_OK; };
	public:
		SimplePromise(std::function<HRESULT(void)> task) : __task(std::move(task)) { }
		SimplePromise & then(const std::function<void(void)> & onSuccess) {
			__onSuccess = onSuccess;
			return *this;
		}
		SimplePromise & fail(const std::function<void(void)> & onFailure) {
			__onFailure = onFailure;
			return *this;
		}
		virtual ~SimplePromise() {
			if (SUCCEEDED(__task())) __onSuccess();
			else __onFailure();
		}
	};
}
