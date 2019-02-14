#include <Fogo.h>
#include <utility>
#include "./Scenes/MainScene.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;

	class MainScene1 : public MainScene {
	public:
		void initialize() override {
			std::cout << "[MainScene1] initialized" << std::endl;
			MainScene::initialize();
		}
		void update() override {
			if (Input::GetTrigger(KeyCode::Return)) {
				System::SetNext("MainScene2");
				System::LoadNext();
			}
			if (Input::GetTrigger(KeyCode::ESCAPE)) {
				PubSub<System::Event, void>::Publish(System::Event::End);
			}
			if (System::IsNextSceneInitialized()) {
				PubSub<System::Event, void>::Publish(System::Event::Next);
			}
			MainScene::update();
		}
		void finalize() override {
			std::cout << "[MainScene1] finalized" << std::endl;
			MainScene::finalize();
		}
	};

	class MainScene2 : public MainScene {
	public:
		void initialize() override {
			std::cout << "[MainScene2] initialized" << std::endl;
			MainScene::initialize();
		}
		void update() override {
			if (Input::GetTrigger(KeyCode::Return)) {
				System::SetNext("MainScene1");
				System::LoadNext();
			}
			if (Input::GetTrigger(KeyCode::ESCAPE)) {
				PubSub<System::Event, void>::Publish(System::Event::End);
			}
			if (System::IsNextSceneInitialized()) {
				PubSub<System::Event, void>::Publish(System::Event::Next);
			}
			MainScene::update();
		}
		void finalize() override {
			std::cout << "[MainScene2] finalized" << std::endl;
			MainScene::finalize();
		}
	};

	Initialize(
		Fogo::Properties()
		.setScenes({
			{ "MainScene1", new MainScene1 },
			{ "MainScene2", new MainScene2 },
		})
		.setFirstSceneKey("MainScene1")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
