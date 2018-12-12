#include <Fogo.h>
#include <utility>
#include "./Scenes/MainScene.h"
#include "./Square.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;

	struct HiroyukiScene : Fogo::Game::Scene {
		auto initialize() -> void override {
			components.emplace_back(std::make_shared<Square>(Square::Option{ std::make_shared<Texture>(L"./resources/b.png") }));
			Scene::initialize();
		}
		auto update() -> void override {
			if (Input::GetTrigger(KeyCode::Return)) {
				System::SetNext("MainScene");
				PubSub<System::Event, void>::Publish(System::Event::Next);
			}
			Scene::update();
		}
	};

	Initialize(
		Fogo::Properties()
		.setScenes({
			{ "HiroyukiScene", std::make_shared<HiroyukiScene>() },
			{ "MainScene", std::make_shared<MainScene>() }
		})
		.setFirstSceneKey("MainScene")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
