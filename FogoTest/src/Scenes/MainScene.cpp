#include "MainScene.h"
#include "../Store.h"
#include "../Components/Camera.h"
#include <iostream>

using Microsoft::WRL::ComPtr;
using Fogo::Graphics::DX12::Graphics;
using Fogo::Graphics::DX12::Texture;
using Fogo::Graphics::DX12::FBXModel;
using Fogo::Utility::Input;
using Fogo::Utility::KeyCode;
using Fogo::Utility::PubSub;
using Fogo::Utility::MappedStore;
using Fogo::Game::System;
using Fogo::Game::Store;
using Fogo::Game::Component;
using Fogo::Game::Updater;
using Fogo::Game::Renderer;
using Fogo::Game::Initializer;
using Fogo::Game::Finalizer;
using Fogo::Game::Meta;

MainScene::MainScene() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}
	const auto & camera = create<Camera>().makeIndex("MainCamera");

	using namespace DirectX;

	camera->target = XMVECTOR { 0, 0, 0 };
	camera->position = XMVECTOR { 0, 0, -50 };
	camera->fov = 60;
	camera->nearZ = 1;
	camera->farZ = 1000;
}
