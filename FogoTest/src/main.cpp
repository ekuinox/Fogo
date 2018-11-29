#include <Fogo.h>
#include <iostream>
#include "Square.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo;

	Window::Create(800, 640, [](HWND handle, UINT message, const WPARAM wParam, const LPARAM lParam) -> HRESULT {
		if (message == WM_DESTROY) PostQuitMessage(0);
		if (message == WM_KEYDOWN && wParam == VK_ESCAPE) DestroyWindow(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}, L"TITLE", L"CLASS");

	Window::HideConsole();

	DX12Graphics::Create(Window::GetHandle(), { Window::GetWidth(), Window::GetHeight() });

	const auto & square = Square(DX12Graphics::GetDevice(), Square::Option { });

	DX12Graphics::Render({
		[&](ID3D12GraphicsCommandList * commandList) {
			square.render(commandList);
		}
	});

	Window::GetInstance().run();

	return 0;
}
