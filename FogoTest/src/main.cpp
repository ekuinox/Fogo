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

	using ComponentStore = TreeStore<void, int, std::shared_ptr<Square>, std::shared_ptr<Car>, std::shared_ptr<FBXSample>>;

	enum class VertexShader { BOX };
	enum class PixelShader { BOX };

	using ShaderStore = MappedStore<
		VertexShader, ComPtr<ID3DBlob>,
		PixelShader, ComPtr<ID3DBlob>
	>;

	ShaderStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
	ShaderStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));

	ComponentStore::Insert(0, -1, std::shared_ptr<FBXSample>(new FBXSample(
		"./resources/box.fbx",
		ShaderStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX),
		ShaderStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX)
	)));

	const auto scene = std::make_shared<Fogo::Game::Scene>();
	scene->components.emplace_back(ComponentStore::Get<std::shared_ptr<FBXSample>>(0));

	const auto gameController = Fogo::Game::GameController({ scene });

	Window::GetInstance().run();

	return 0;
}
