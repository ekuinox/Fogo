#include <Fogo.h>
#include <iostream>

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo;

	Window::Create(800, 640, [](HWND handle, UINT message, const WPARAM wParam, const LPARAM lParam) -> HRESULT {
		if (message == WM_DESTROY) PostQuitMessage(0);
		if (message == WM_KEYDOWN && wParam == VK_ESCAPE) DestroyWindow(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}, L"TITLE", L"CLASS");

	Window::HideConsole();

	Window::GetInstance().run();

	return 0;
}
