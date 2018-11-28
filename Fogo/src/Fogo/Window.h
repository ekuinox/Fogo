#pragma once

#include <windows.h>
#include <functional>
#include "SingletonWrapper.h"
#pragma comment(lib, "winmm.lib")	

namespace Fogo {

	class _Window
	{
	public:
		using Procedure = std::function<HRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)>;
	private:
		HINSTANCE __instance;
		HWND wh;
		WNDCLASSEX __windowClass;
		struct
		{
			int width, height;
		} size;

		class create_window_class_exeption : public std::exception
		{
		public:
			create_window_class_exeption() : std::exception("create_window_class_exeption") {}
		};

		auto createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) -> WNDCLASSEX const
		{
			auto windowClass = WNDCLASSEX
			{
				sizeof(WNDCLASSEX),
				0,
				procedure,
				0,
				0,
				__instance,
				LoadIcon((HINSTANCE)NULL, IDI_APPLICATION),
				LoadCursor((HINSTANCE)NULL, IDC_ARROW),
				(HBRUSH)GetStockObject(WHITE_BRUSH),
				0,
				className,
				LoadIcon((HINSTANCE)NULL, IDI_WINLOGO)
			};

			if (!RegisterClassEx(&windowClass)) throw create_window_class_exeption();

			return windowClass;
		}

		auto createWindow(const LPCWSTR & title) -> HWND const
		{
			return CreateWindow(
				__windowClass.lpszClassName,
				title,
				WS_CAPTION | WS_SYSMENU,
				0,
				0,
				size.width,
				size.height,
				HWND_DESKTOP,
				(HMENU)NULL,
				__instance,
				(LPVOID)NULL
			);
		}

		auto moveWindowCenter() -> void const
		{
			RECT windowRect, clientRect;
			GetWindowRect(wh, &windowRect);
			GetClientRect(wh, &clientRect);
			const auto width = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + size.width;
			const auto height = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + size.height;

			SetWindowPos(
				wh,
				NULL,
				GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
				GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
				width - 1,
				height - 1,
				SWP_NOZORDER
			);
		}


	public:
		_Window(const int width, const int height, const WNDPROC & procedure = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
			if (message == WM_DESTROY) PostQuitMessage(0);
			return DefWindowProc(hwnd, message, wParam, lParam);
		}, const LPCWSTR & title = L"DEFAULT_TITLE", const LPCWSTR & className = L"DEFAULT_CLASS_NAME")
			: __instance(GetModuleHandle(0)),
			size({width, height}),
			__windowClass(createWindowClass(procedure, className))
		{
			wh = createWindow(title);

			moveWindowCenter();
		}

		auto run() const -> int
		{
			ShowWindow(wh, SW_NORMAL);
			UpdateWindow(wh);

			MSG msg;
			while (true)
			{
				if (!GetMessage(&msg, NULL, 0, 0))
				{
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			return static_cast<int>(msg.wParam);
		}

		auto getHandle() const -> HWND
		{
			return wh;
		}

		static auto hideConsole() -> void
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
	};

	using Window = SingletonWrapper<_Window>;

} // namespace Fogo
