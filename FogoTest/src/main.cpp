#include <Fogo.h>
#include <utility>
#include "./Scenes/MainScene.h"
#include "./Square.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;

	struct SampleScene : Fogo::Game::Scene {
		const char * name;
		LPCWSTR textureFile;
		SampleScene(const char * name, LPCWSTR textureFile) : name(name), textureFile(textureFile) {}
		auto initialize() -> void override {
			components.emplace_back(std::make_shared<Square>(Square::Option { std::make_shared<Texture>(textureFile) }));
			Scene::initialize();
		}
		auto update() -> void override {
			if (Input::GetTrigger(KeyCode::Return)) {
				System::SetNext(name);
				System::LoadNext();
			}
			if (System::IsNextSceneInitialized()) {
				PubSub<System::Event, void>::Publish(System::Event::Next);
			}
			Scene::update();
		}
	};

	Initialize(
		Fogo::Properties()
		.setScenes({
			{ "HiroyukiScene", std::make_shared<SampleScene>("HutariScene", L"./resources/b.png") },
			{ "HutariScene", std::make_shared<SampleScene>("HiroyukiScene", L"./resources/a.png") }
		})
		.setFirstSceneKey("HiroyukiScene")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
