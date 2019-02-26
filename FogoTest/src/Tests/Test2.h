#pragma once

#include <Fogo.h>
#include "../Scenes/MainScene.h"

struct SceneKey {
	static constexpr auto MainScene1 = "MainScene1";
	static constexpr auto MainScene2 = "MainScene2";
};

inline void test2() {

	using namespace Fogo;
	using Game::Scene;
	using Game::System;
	using Utility::KeyCode;
	using Utility::Input;
	using Utility::PubSub;

	class MainScene1 : public MainScene {
	public:
		void initialize() override {
			std::cout << "[MainScene1] initialized" << std::endl;
			MainScene::initialize();
		}
		void update() override {
			if (Input::GetTrigger(KeyCode::Return)) {
				System::SetNext(SceneKey::MainScene2);
				System::LoadNext();
			}
			if (Input::GetTrigger(KeyCode::Escape)) {
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
				System::SetNext(SceneKey::MainScene1);
				System::LoadNext();
			}
			if (Input::GetTrigger(KeyCode::Escape)) {
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
		Properties()
			.setCreateScenes([](System & system) {
				system.create<MainScene1>()
					.makeIndex(SceneKey::MainScene1) // MainScene�ɑ΂���
					.makeIndex<Scene>(SceneKey::MainScene1); // Scene�ɑ΂���
				system.create<MainScene2>()
					.makeIndex(SceneKey::MainScene2)
					.makeIndex<Scene>(SceneKey::MainScene2);
			})
			.setFirstSceneKey(SceneKey::MainScene1)
			.setWidth(800)
			.setHeight(640)
			.setTitle(L"FogoTest")
	);
}
