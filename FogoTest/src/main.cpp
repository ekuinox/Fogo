#include <Fogo.h>
#include <iostream>
#include "Square.h"
#include <thread>

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;

	Window::Create(800, 640, [](HWND handle, UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
		if (message == WM_DESTROY) PostQuitMessage(0);
		if (message == WM_KEYDOWN && wParam == VK_ESCAPE) DestroyWindow(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}, L"TITLE", L"CLASS");

	Graphics::Create(Window::GetHandle(), { Window::GetWidth(), Window::GetHeight() });

	enum class TextureType {
		HIROYUKI
	};

	using ResourceStore = MappedStore<TextureType, std::shared_ptr<Texture>>;

	ResourceStore::Insert(TextureType::HIROYUKI, std::make_shared<Texture>(Graphics::GetDevice(), L"resources/b.png"));

	std::vector<std::unique_ptr<Square>> squares;

	squares.emplace_back(std::make_unique<Square>(Graphics::GetDevice(), Square::Option { {  1.0f, 0.0f }, { 2.0f, 2.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(TextureType::HIROYUKI) }));
	squares.emplace_back(std::make_unique<Square>(Graphics::GetDevice(), Square::Option { { -2.0f, 0.0f }, { 1.0f, 1.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(TextureType::HIROYUKI) }));

	bool isLoop = true;
	std::thread th([&] {
		while(isLoop) {
			Time::Start();
			for (const auto & square : squares) square->update();
			Graphics::Render({ [&](Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) { for (const auto & square : squares) square->render(commandList); } });
			Time::Stop();
		}
	});

	Window::GetInstance().run();

	isLoop = false;
	th.join();

	return 0;
}
