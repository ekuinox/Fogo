#include "MainScene.h"
#include <iostream>

using Fogo::Graphics::DX12::Graphics;
using Fogo::Graphics::DX12::Texture;
using Microsoft::WRL::ComPtr;

void MainScene::initialize() {
	ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
	ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
	ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));

	ComponentStore::Insert(0, -1, std::shared_ptr<FBXSample>(new FBXSample(
		"./resources/2.fbx",
		ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX),
		ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX),
		ResourceStore::Get<std::shared_ptr<Texture>>(TextureType::BOX)
	)));

	ComponentStore::Insert(1, -1, std::make_shared<InputDebugger>());

	components.emplace_back(ComponentStore::Get<std::shared_ptr<FBXSample>>(0));
	components.emplace_back(ComponentStore::Get<std::shared_ptr<InputDebugger>>(1));
}
