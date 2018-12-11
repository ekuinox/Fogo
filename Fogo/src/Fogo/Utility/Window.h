#pragma once

#include <windows.h>
#include <functional>
#include <utility>
#include "PubSub.h"

namespace Fogo::Utility {

	class Window
	{
	public:
		struct create_window_class_exception : std::exception { create_window_class_exception() : std::exception("create_window_class_exception") {} };

		static constexpr LPCWSTR DEFAULT_TITLE = L"DEFAULT_TITLE";
		static constexpr LPCWSTR DEFAULT_CLASS_NAME = L"DEFAULT_CLASS_NAME";

		enum class Event {
			OnDestroy
		};

	private:
		HINSTANCE __instance_handle;
		HWND __window_handle;
		WNDCLASSEX __window_class;
		struct { UINT width, height; } __window_size;

		static Window * instance;
		static WNDPROC default_procedure;

		auto createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX;
		auto createWindow(const LPCWSTR & title) const -> HWND;
		auto moveWindowCenter() const -> void;

		Window(
			const UINT width,
			const UINT height,
			const WNDPROC & procedure = default_procedure,
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		);
	public:
		struct Properties {
			UINT width;
			UINT height;
			WNDPROC procedure = default_procedure;
			LPCWSTR title = DEFAULT_TITLE;
			LPCWSTR className = DEFAULT_CLASS_NAME;
			Properties & setWidth(const UINT & newWidth) { width = newWidth; return *this; }
			Properties & setHeight(const UINT & newHeight) { height = newHeight; return * this; }
			Properties & setProcedure(const WNDPROC & newProcedure) { procedure = newProcedure; return * this; }
			Properties & setTitle(const LPCWSTR & newTitle) { title = newTitle; return *this; }
			Properties & setClassName(const LPCWSTR & newClassName) { className = newClassName; return * this; }
		};

		Window(const Window &) = delete;
		auto operator=(const Window &) -> Window & = delete;
		auto run() const -> int;
		static auto HideConsole() -> void;
		static auto Create(const Properties & properties) -> Window &;
		static auto Create(
			const UINT width,
			const UINT height,
			const WNDPROC & procedure = default_procedure,
			const LPCWSTR & title = DEFAULT_TITLE,
			const LPCWSTR & className = DEFAULT_CLASS_NAME
		) -> Window &;
		static auto Destroy() -> void;
		static auto GetWidth() -> UINT;
		static auto GetHeight() -> UINT;
		static auto GetHandle() -> HWND;
		static auto GetWindowHandle() -> HWND;
		static auto GetInstanceHandle() -> HINSTANCE;
		static auto GetInstance() -> Window &;
	};

} // namespace Fogo
