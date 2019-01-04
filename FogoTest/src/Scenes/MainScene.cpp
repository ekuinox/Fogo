#include "MainScene.h"
#include <iostream>

using Fogo::Graphics::DX12::Graphics;
using Fogo::Graphics::DX12::Texture;
using Fogo::Utility::Input;
using Fogo::Utility::KeyCode;
using Fogo::Utility::PubSub;
using Fogo::Game::GameController;
using Microsoft::WRL::ComPtr;

void MainScene::initialize() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}

	ComponentStore::Insert(0, -1, std::shared_ptr<FBX>(new FBX()));
	ComponentStore::Get<std::shared_ptr<FBX>>(0)->modelFile = "./resources/model/unitychan.fbx";
	ComponentStore::Get<std::shared_ptr<FBX>>(0)->vertexShader = ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX);
	ComponentStore::Get<std::shared_ptr<FBX>>(0)->pixelShader = ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX);

	ComponentStore::Insert(1, -1, std::make_shared<InputDebugger>());

	components.emplace_back(ComponentStore::Get<std::shared_ptr<FBX>>(0));
	components.emplace_back(ComponentStore::Get<std::shared_ptr<InputDebugger>>(1));

	Scene::initialize();
}

void MainScene::update() {
	Scene::update();
}

void MainScene::finalize() {
	components.clear();
	ComponentStore::Execute<std::shared_ptr<Fogo::Game::ComponentInterface>>([&] (std::shared_ptr<Fogo::Game::ComponentInterface> component) {
		component.reset();
	});
	Scene::finalize();
}
