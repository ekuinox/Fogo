#pragma once

#include <Fogo.h>
#include <fstream>
#include "../Scenes/MainScene.h"
#include "../Store.h"
#include "../Components/Model.h"

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
		MainScene1() : MainScene() {
			create<Fogo::Game::Meta>("MainScene1", "MainScene1", 0);
			create<Model>(
				"./resources/2.fbx",
				ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(VertexShader::BOX),
				ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(PixelShader::BOX)
			).makeIndex("Box");
		}
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
			if (Input::GetTrigger(KeyCode::P)) {
				std::ofstream output("./componentTree.json", std::ios_base::trunc);
				Fogo::Debug::DumpComponent(output, System::GetInstance(), 0, '\t', 1);
				Fogo::Debug::Console::Log(System::GetInstance());
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
		MainScene2() : MainScene() {
			create<Fogo::Game::Meta>("MainScene2", "MainScene2", 0);
			create<Model>(
				"./resources/model/unitychan.fbx",
				ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(VertexShader::BOX),
				ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(PixelShader::BOX)
			).makeIndex("UnityChan");
		}
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
					.makeIndex(SceneKey::MainScene1) // MainScene‚É‘Î‚µ‚Ä
					.makeIndex<Scene>(SceneKey::MainScene1); // Scene‚É‘Î‚µ‚Ä
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
