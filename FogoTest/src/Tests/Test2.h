#pragma once

#include <Fogo.h>
#include "../Scenes/MainScene.h"

inline void test2() {

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
		.setCreateScenes([](System & system) {
			system.create<MainScene1>().makeIndex(static_cast<const char *>("MainScene1"));
			Fogo::Game::Store::Get<Fogo::Game::Scene>(system.get<MainScene1>(static_cast<const char*>("MainScene1"))->uuid).get().value().makeIndex(static_cast<const char *>("MainScene1"));
			system.create<MainScene2>().makeIndex(static_cast<const char *>("MainScene2"));
			Fogo::Game::Store::Get<Fogo::Game::Scene>(system.get<MainScene2>(static_cast<const char*>("MainScene2"))->uuid).get().value().makeIndex(static_cast<const char *>("MainScene2"));
		})
		.setFirstSceneKey("MainScene1")
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
}
