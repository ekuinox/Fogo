#include <Fogo.h>
#include <utility>
#include "./Scenes/MainScene.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;
	struct TScene : Fogo::Game::Scene {
		auto update() -> void override {
			if (Input::GetAnyTrigger()) {
				PubSub<System::Event, void>::Publish(System::Event::Next);
			}
			Scene::update();
		}
	};

	Initialize(
		Fogo::Properties()
		.setScenes({
			{ "TScene", std::make_shared<TScene>() },
			{ "MainScene", std::make_shared<MainScene>() }
		})
		.setFirstSceneKey("MainScene")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
