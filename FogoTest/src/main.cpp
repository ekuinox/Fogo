#include <Fogo.h>
#include <iostream>
#include "Square.h"
#include <thread>
#include "Fogo/Game/GameController.h"
#include "Car.h"

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
		Car1,
	};

	using ResourceStore = MappedStore<Textures, std::shared_ptr<Texture>>;
	using ComponentStore = TreeStore<void, Components, std::shared_ptr<Square>, std::shared_ptr<Car>>;

	ResourceStore::Insert(Textures::HIROYUKI, std::make_shared<Texture>(L"resources/b.png"));
	ResourceStore::Insert(Textures::FUTARI, std::make_shared<Texture>(L"resources/a.png"));

	ComponentStore::Insert(Components::SQUARE1, Components::NOTHING, std::make_shared<Square>(Square::Option { {  1.0f, 0.0f }, { 2.0f, 2.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(Textures::HIROYUKI) }));
	ComponentStore::Insert(Components::SQUARE2, Components::NOTHING, std::make_shared<Square>(Square::Option { { -2.0f, 0.0f }, { 1.0f, 1.0f }, ResourceStore::Get<std::shared_ptr<Texture>>(Textures::FUTARI) }));
	ComponentStore::Insert(Components::Car1, Components::NOTHING, std::make_shared<Car>("resources/model/unitychan.fbx"));

	auto scene = std::make_shared<Fogo::Game::Scene>();
	// scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<Square>>(Components::SQUARE1));
	// scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<Square>>(Components::SQUARE2));
	scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<Car>>(Components::Car1));

	const auto gameController = Fogo::Game::GameController({ scene });

	Window::GetInstance().run();

	return 0;
}
