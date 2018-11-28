#include "Window.h"

#pragma comment(lib, "winmm.lib")	

auto Fogo::Window::createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX
{
	auto windowClass = WNDCLASSEX
	{
		sizeof(WNDCLASSEX),
		0,
		procedure,
		0,
		0,
		__instance,
		LoadIcon(static_cast<HINSTANCE>(nullptr), IDI_APPLICATION),
		LoadCursor(static_cast<HINSTANCE>(nullptr), IDC_ARROW),
		static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
		nullptr,
		className,
		LoadIcon(static_cast<HINSTANCE>(nullptr), IDI_WINLOGO)
	};

	if (!RegisterClassEx(&windowClass)) throw create_window_class_exception();

	return windowClass;
}

auto Fogo::Window::createWindow(const LPCWSTR & title) const -> HWND
{
	return CreateWindow(
		__window_class.lpszClassName,
		title,
		WS_CAPTION | WS_SYSMENU,
		0,
		0,
		__window_size.width,
		__window_size.height,
		HWND_DESKTOP,
		static_cast<HMENU>(nullptr),
		__instance,
		static_cast<LPVOID>(nullptr)
	);
}

auto Fogo::Window::moveWindowCenter() const -> void
{
	RECT windowRect, clientRect;
	GetWindowRect(__window_handle, &windowRect);
	GetClientRect(__window_handle, &clientRect);
	const auto width = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + __window_size.width;
	const auto height = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + __window_size.height;

	SetWindowPos(
		__window_handle,
		nullptr,
		GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
		width - 1,
		height - 1,
		SWP_NOZORDER
	);
}

Fogo::Window::Window(const int width, const int height, const WNDPROC & procedure, const LPCWSTR & title, const LPCWSTR & className)
	: __instance(GetModuleHandle(nullptr)), __window_size({ width, height }), __window_class(createWindowClass(procedure, className))
{
	__window_handle = createWindow(title);

	moveWindowCenter();
}

auto Fogo::Window::run() const -> int
{
	ShowWindow(__window_handle, SW_NORMAL);
	UpdateWindow(__window_handle);

	MSG msg;
	while (true)
	{
		if (!GetMessage(&msg, nullptr, 0, 0)) break;
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

auto Fogo::Window::getHandle() const -> HWND
{
	return __window_handle;
}

auto Fogo::Window::HideConsole() -> void
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}
