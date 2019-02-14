#include "MainScene.h"
#include <iostream>

using Fogo::Graphics::DX12::Graphics;
using Fogo::Graphics::DX12::Texture;
using Fogo::Utility::Input;
using Fogo::Utility::KeyCode;
using Fogo::Utility::PubSub;
using Fogo::Game::System;
using Microsoft::WRL::ComPtr;
using Fogo::Game::Store;
using Fogo::Game::Component;
using Fogo::Game::LifeCycled;
using Fogo::Game::Updatable;
using Fogo::Game::Renderable;

enum class Key {
	FBX1,
	InputDebugger1
};

enum class VertexShader { BOX };
enum class PixelShader { BOX };
enum class TextureType { BOX };
using ResourceStore = Fogo::Utility::MappedStore<
	VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	TextureType, std::shared_ptr<Fogo::Graphics::DX12::Texture>
>;

struct InputDebugger : Component, LifeCycled {
	// nothing
};

struct FBX : Component, LifeCycled, Updatable, Renderable {
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
	const char * modelFile;
	std::unique_ptr<Fogo::Graphics::DX12::FBXModel> model;
	void initialize() override {
		model = std::make_unique<Fogo::Graphics::DX12::FBXModel>(
			modelFile,
			Fogo::Graphics::DX12::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
			.setPixelShader(pixelShader).setVertexShader(vertexShader)
		);
		model->matrix = DirectX::XMMatrixIdentity();
		LifeCycled::initialize();
	}
	void update() override {
		using namespace DirectX;
		using namespace Fogo::Utility;
		static constexpr auto SPEED = 10.0f;

		XMFLOAT3 translation{ 0, 0, 0 };
		XMFLOAT3 rotation{ 0, 0, 0 };

		if (Input::GetPress(KeyCode::A)) rotation.y -= Time::GetElapsedTime() * SPEED;
		if (Input::GetPress(KeyCode::D)) rotation.y += Time::GetElapsedTime() * SPEED;
		if (Input::GetPress(KeyCode::W)) translation.z += Time::GetElapsedTime() * SPEED;
		if (Input::GetPress(KeyCode::S)) translation.z -= Time::GetElapsedTime() * SPEED;
		if (Input::GetPress(KeyCode::LShift)) translation.y -= Time::GetElapsedTime() * SPEED;
		if (Input::GetPress(KeyCode::Space)) translation.y += Time::GetElapsedTime() * SPEED;

		const auto rotation_matrix
			= XMMatrixRotationX(XMConvertToRadians(rotation.x * Time::GetElapsedTime() * 360))
			* XMMatrixRotationY(XMConvertToRadians(rotation.y * Time::GetElapsedTime() * 360))
			* XMMatrixRotationZ(XMConvertToRadians(rotation.z * Time::GetElapsedTime() * 360));

		const auto translation_matrix = XMMatrixTranslation(translation.x, translation.y, translation.z);

		model->matrix = rotation_matrix * translation_matrix * model->matrix;
	}
	void render() const override {
		model->render();
	}
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
