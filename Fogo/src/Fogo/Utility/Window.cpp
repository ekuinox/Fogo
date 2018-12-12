#include "Window.h"

#pragma comment(lib, "winmm.lib")	

using namespace Fogo::Utility;

Window * Window::instance = nullptr;

WNDPROC Window::default_procedure = [](HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		Fogo::Utility::PubSub<Fogo::Utility::Window::Event, void>::Publish(Fogo::Utility::Window::Event::OnDestroy);
	}
	return DefWindowProc(handle, message, wParam, lParam);
};

auto Window::createWindowClass(const WNDPROC & procedure, const LPCWSTR & className) const -> WNDCLASSEX {
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
		__is_full_screen ? (WS_VISIBLE | WS_POPUP) : (WS_CAPTION | WS_SYSMENU),
		0,
		0,
		__window_size.width,
		__window_size.height,
		__is_full_screen ? NULL : HWND_DESKTOP,
		static_cast<HMENU>(nullptr),
		__instance_handle,
		static_cast<LPVOID>(nullptr)
	);
}

auto Window::moveWindowCenter() const -> void
{
	if (__is_full_screen) return;
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

Window::Window(
	const UINT width,
	const UINT height,
	const WNDPROC & procedure,
	const LPCWSTR & title,
	const LPCWSTR & className,
	bool isFullScreen)
: __instance_handle(GetModuleHandle(nullptr))
, __window_size({ width, height })
, __window_class(createWindowClass(procedure, className))
, __is_full_screen(isFullScreen)
{
	__window_handle = createWindow(title);

	moveWindowCenter();
}

auto Window::run() const -> int
{
	ShowWindow(__window_handle, SW_NORMAL);
	UpdateWindow(__window_handle);

	MSG msg;
	while (__running)
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
		instance = new Window(
			properties.width,
			properties.height,
			properties.procedure,
			properties.title,
			properties.className,
			properties.isFullScreen
		);
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

auto Window::Stop() -> void {
	GetInstance().__running = false;
}
