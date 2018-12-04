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

	enum class Textures {
		HIROYUKI,
		FUTARI
	};

	enum class Components {
		NOTHING,
		SQUARE1,
		SQUARE2,

	};

	using ResourceStore = MappedStore<Textures, std::shared_ptr<Texture>>;
	using ComponentStore = TreeStore<void, Components, std::shared_ptr<Square>>;

	ResourceStore::Insert(Textures::HIROYUKI, std::make_shared<Texture>(Graphics::GetDevice(), L"resources/b.png"));
	ResourceStore::Insert(Textures::FUTARI, std::make_shared<Texture>(Graphics::GetDevice(), L"resources/a.png"));

	ComponentStore::Insert(Components::SQUARE1, Components::NOTHING, std::make_shared<Square>(Graphics::GetDevice(), Square::Option { {  1.0f, 0.0f }, { 2.0f, 2.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(Textures::HIROYUKI) }));
	ComponentStore::Insert(Components::SQUARE2, Components::NOTHING, std::make_shared<Square>(Graphics::GetDevice(), Square::Option { { -2.0f, 0.0f }, { 1.0f, 1.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(Textures::FUTARI) }));

	bool isLoop = true;
	std::thread th([&] {
		while(isLoop) {
			Time::Start();
			ComponentStore::Execute<std::shared_ptr<Square>>([&](std::shared_ptr<Square> square) { square->update(); });
			Graphics::Render({ [&](Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) {
				ComponentStore::Execute<std::shared_ptr<Square>>([&](std::shared_ptr<Square> square) { square->render(commandList); });
			} });
			Time::Stop();
		}
	});

	Window::GetInstance().run();

	isLoop = false;
	th.join();

	return 0;
}
