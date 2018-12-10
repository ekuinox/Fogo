#pragma once

#include <windows.h>
#include <functional>
#include <utility>

namespace Fogo::Utility {

	class Window
	{
	public:
		struct create_window_class_exception : std::exception { create_window_class_exception() : std::exception("create_window_class_exception") {} };

		static constexpr LPCWSTR DEFAULT_TITLE = L"DEFAULT_TITLE";
		static constexpr LPCWSTR DEFAULT_CLASS_NAME = L"DEFAULT_CLASS_NAME";

		class SimplePromise {
			std::function<void(void)> __onSuccess = [] {};
			std::function<void(void)> __onFailure = [] {};
			std::function<HRESULT(void)> __task = [] { return S_OK; };
		public:
			SimplePromise(std::function<HRESULT(void)> task) : __task(std::move(task)) { }
			SimplePromise & then(const std::function<void(void)> & onSuccess) {
				__onSuccess = onSuccess;
				return * this;
			}
			SimplePromise & fail(const std::function<void(void)> & onFailure) {
				__onFailure = onFailure;
				return * this;
			}
			virtual ~SimplePromise() {
				if (SUCCEEDED(__task())) __onSuccess();
				else __onFailure();
			}
		};

	private:
		HINSTANCE __instance;
		HWND __window_handle;
		WNDCLASSEX __window_class;
		struct { UINT width, height; } __window_size;

		static Window * instance;

		auto createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX;
		auto createWindow(const LPCWSTR & title) const -> HWND;
		auto moveWindowCenter() const -> void;

		Window(
			const UINT width,
			const UINT height,
			const WNDPROC & procedure = [](HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
				if (message == WM_DESTROY) PostQuitMessage(0);
				return DefWindowProc(handle, message, wParam, lParam);
			},
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		);
	public:
		Window(const Window &) = delete;
		auto operator=(const Window &) -> Window & = delete;
		auto run() const -> int;
		static auto HideConsole() -> void;
		static auto Create(
			const UINT width,
			const UINT height,
			const WNDPROC & procedure = [](HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
				if (message == WM_DESTROY) PostQuitMessage(0);
				return DefWindowProc(handle, message, wParam, lParam);
			},
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		) -> Window &;
		static auto Destroy() -> void;
		static auto GetWidth() -> UINT;
		static auto GetHeight() -> UINT;
		static auto GetHandle() -> HWND;
		static auto GetInstance() -> Window &;
	};

} // namespace Fogo
