#include <Fogo.h>
#include <utility>
#include "./Scenes/MainScene.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;

	Initialize(
		Fogo::Properties()
		.setScenes({
			{ "MainScene", std::make_shared<MainScene>() },
		})
		.setFirstSceneKey("MainScene")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
