#pragma once

#include <windows.h>
#include <functional>

namespace Fogo {

	class Window
	{
	public:
		struct create_window_class_exception : std::exception { create_window_class_exception() : std::exception("create_window_class_exception") {} };

		static constexpr LPCWSTR DEFAULT_TITLE = L"DEFAULT_TITLE";
		static constexpr LPCWSTR DEFAULT_CLASS_NAME = L"DEFAULT_CLASS_NAME";

	private:
		HINSTANCE __instance;
		HWND __window_handle;
		WNDCLASSEX __window_class;
		struct { int width, height; } __window_size;

		static Window * instance;

		auto createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX;
		auto createWindow(const LPCWSTR & title) const -> HWND;
		auto moveWindowCenter() const -> void;

		Window(
			const int width,
			const int height,
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
		auto getHandle() const -> HWND;
		static auto HideConsole() -> void;
		static auto Create(
			const int width,
			const int height,
			const WNDPROC & procedure = [](HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
				if (message == WM_DESTROY) PostQuitMessage(0);
				return DefWindowProc(handle, message, wParam, lParam);
			},
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		) -> Window &;
		static auto Destroy() -> void;
		static auto GetInstance() -> Window &;
	};

} // namespace Fogo
