#include "Window.h"

#pragma comment(lib, "winmm.lib")	

using namespace Fogo::Utility;

Window * Window::instance = nullptr;

auto Window::createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX
{
	auto windowClass = WNDCLASSEX
	{
		sizeof(WNDCLASSEX),
		0,
		procedure,
		0,
		0,
		__instance_handle,
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

auto Window::createWindow(const LPCWSTR & title) const -> HWND
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
		__instance_handle,
		static_cast<LPVOID>(nullptr)
	);
}

auto Window::moveWindowCenter() const -> void
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

Window::Window(const UINT width, const UINT height, const WNDPROC & procedure, const LPCWSTR & title, const LPCWSTR & className)
	: __instance_handle(GetModuleHandle(nullptr)), __window_size({ width, height }), __window_class(createWindowClass(procedure, className))
{
	__window_handle = createWindow(title);

	moveWindowCenter();
}

auto Window::run() const -> int
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

auto Window::HideConsole() -> void
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

auto Window::Create(const Properties & properties) -> Window & {
	if (instance == nullptr)
		instance = new Window(properties.width, properties.height, properties.procedure, properties.title, properties.className);
	return * instance;
}

auto Window::Create(const UINT width, const UINT height, const WNDPROC & procedure, const LPCWSTR & title, const LPCWSTR & className) -> Window & {
	if (instance == nullptr) instance = new Window(width, height, procedure, title, className);
	return * instance;
}

auto Window::Destroy() -> void {
	delete instance;
	instance = nullptr;
}

auto Window::GetWidth() -> UINT
{
	return GetInstance().__window_size.width;
}

auto Window::GetHeight() -> UINT
{
	return GetInstance().__window_size.height;
}

auto Window::GetHandle() -> HWND
{
	return GetInstance().__window_handle;
}

auto Window::GetWindowHandle() -> HWND
{
	return GetInstance().__window_handle;
}

auto Window::GetInstanceHandle() -> HINSTANCE
{
	return GetInstance().__instance_handle;
}


auto Window::GetInstance() -> Window & {
	return * instance;
}
