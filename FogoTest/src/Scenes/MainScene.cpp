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


	store.insert("fbx", std::shared_ptr<FBX>(new FBX()));
	store.get<std::shared_ptr<FBX>>("fbx")->modelFile = "./resources/2.fbx";
	store.get<std::shared_ptr<FBX>>("fbx")->vertexShader = ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX);
	store.get<std::shared_ptr<FBX>>("fbx")->pixelShader = ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX);

	store.insert("inputDebugger", std::make_shared<InputDebugger>());

	components.emplace_back(store.get<std::shared_ptr<FBX>>("fbx"));
	components.emplace_back(store.get<std::shared_ptr<InputDebugger>>("inputDebugger"));

	Scene::initialize();
}

void MainScene::update() {
	Scene::update();
}

void MainScene::finalize() {
	components.clear();
	store.execute<std::shared_ptr<FBX>>([&](std::shared_ptr<FBX> component) { component.reset(); });
	store.execute<std::shared_ptr<InputDebugger>>([&](std::shared_ptr<InputDebugger> component) { component.reset(); });

	Scene::finalize();
}
