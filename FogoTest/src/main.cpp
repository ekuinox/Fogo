#include <Fogo.h>
#include <iostream>
#include "Square.h"
#include <thread>
#include "Fogo/Game/GameController.h"
#include "Car.h"
#include "FBXSample.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;

	Window::Create(800, 640, [](HWND handle, UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
		if (message == WM_DESTROY) PostQuitMessage(0);
		if (message == WM_KEYDOWN && wParam == VK_ESCAPE) DestroyWindow(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}, L"TITLE", L"CLASS");

	Graphics::Create(Window::GetHandle(), { Window::GetWidth(), Window::GetHeight() });

	struct InputDebugger : Fogo::Game::ComponentInterface {
		void update() override {
			if (Input::GetPress(MouseButton::Right)) {
				std::cout << "‰EƒNƒŠ" << std::endl;
			}
			if (Input::GetPress(MouseButton::Left)) {
				std::cout << "¶ƒNƒŠ" << std::endl;
			}
			const auto move = Input::GetMouseMove();
			if (!(move.x == 0.0f && move.y == 0.0f && move.z == 0.0f)) {
				std::cout << move.x << ", " << move.y << ", " << move.z << std::endl;
			}
		}
	};

	using ComponentStore = TreeStore<void, int, std::shared_ptr<Square>, std::shared_ptr<Car>, std::shared_ptr<FBXSample>, std::shared_ptr<InputDebugger>>;
	enum class VertexShader { BOX };
	enum class PixelShader { BOX };
	enum class TextureType { BOX };
	using ResourceStore = MappedStore<
		VertexShader, ComPtr<ID3DBlob>,
		PixelShader, ComPtr<ID3DBlob>,
		TextureType, std::shared_ptr<Texture>
	>;

	ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
	ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
	ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));

	ComponentStore::Insert(0, -1, std::shared_ptr<FBXSample>(new FBXSample(
		"./resources/2.fbx",
		ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX),
		ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX),
		ResourceStore::Get<std::shared_ptr<Texture>>(TextureType::BOX)
	)));

	ComponentStore::Insert(1, -1, std::make_shared<InputDebugger>());

	const auto scene = std::make_shared<Fogo::Game::Scene>();
	scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<FBXSample>>(0));
	scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<InputDebugger>>(1));

	const auto gameController = Fogo::Game::GameController({ scene });

	Window::GetInstance().run();

	return 0;
}
