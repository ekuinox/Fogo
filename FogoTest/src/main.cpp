#include <Fogo.h>
#include <iostream>
#include "Square.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;

	Window::Create(800, 640, [](HWND handle, UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
		if (message == WM_DESTROY) PostQuitMessage(0);
		if (message == WM_KEYDOWN && wParam == VK_ESCAPE) DestroyWindow(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}, L"TITLE", L"CLASS");

	Window::HideConsole();

	Graphics::Create(Window::GetHandle(), { Window::GetWidth(), Window::GetHeight() });

	const auto & square = Square(Graphics::GetDevice(), Square::Option { });

	Graphics::Render({
		[&](ID3D12GraphicsCommandList * commandList) {
			square.render(commandList);
		}
	});

	Window::GetInstance().run();

	return 0;
}
