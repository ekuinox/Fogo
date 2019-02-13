#include "MainScene.h"
#include <iostream>

using Fogo::Graphics::DX12::Graphics;
using Fogo::Graphics::DX12::Texture;
using Fogo::Utility::Input;
using Fogo::Utility::KeyCode;
using Fogo::Utility::PubSub;
using Fogo::Game::GameController;
using Microsoft::WRL::ComPtr;
using Fogo::Game::Store;

enum class Key {
	FBX1,
	InputDebugger1
};

void MainScene::initialize() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}

	auto & fbx = create<FBX>().makeIndex(Key::FBX1);

	fbx->modelFile = "./resources/2.fbx";
	fbx->vertexShader = ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX);
	fbx->pixelShader = ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX);

	auto & inputDebugger = create<InputDebugger>().makeIndex(Key::InputDebugger1);

	Scene::initialize();
}

void MainScene::update() {
	Scene::update();
}

void MainScene::finalize() {
	Scene::finalize();
}
