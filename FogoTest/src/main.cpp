#include <Fogo.h>
#include "./Scenes/MainScene.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;

	Fogo::Initialize(
		Window::Properties()
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);

	const auto gameController = Fogo::Game::GameController({ std::make_shared<MainScene>() });
	
	return Window::GetInstance().run();
}
