#include "MainScene.h"
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

enum class Key {
	FBX1,
	InputDebugger1
};

enum class VertexShader { BOX };
enum class PixelShader { BOX };
enum class TextureType { BOX };
using ResourceStore = MappedStore<
	VertexShader, ComPtr<ID3DBlob>,
	PixelShader, ComPtr<ID3DBlob>,
	TextureType, std::shared_ptr<Texture>
>;

class Model : public Component {
	const char * modelFile;
	ComPtr<ID3DBlob> vertexShader, pixelShader;
	std::unique_ptr<FBXModel> model;

	void initialize() {
		model = std::make_unique<Fogo::Graphics::DX12::FBXModel>(
			modelFile,
			Fogo::Graphics::DX12::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
			.setPixelShader(pixelShader).setVertexShader(vertexShader)
			);
		model->matrix = DirectX::XMMatrixIdentity();
	}

	void update() {
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

	void render() const {
		model->render();
	}

public:
	Model(const char * modelFile, ComPtr<ID3DBlob> vertexShader, ComPtr<ID3DBlob> pixelShader) 
	: modelFile(modelFile), vertexShader(vertexShader), pixelShader(pixelShader) {
		create<Initializer>([&] { initialize(); });
		create<Updater>([&] { update(); });
		create<Renderer>([&] { render(); });
	}
};

struct Debugger : Component {
	Debugger() {
		create<Initializer>([&] {
			std::cout << "Debugger initialized" << std::endl;
		});
		create<Finalizer>([&] {
			std::cout << "Debugger finalized" << std::endl;
		});
	}
};

MainScene::MainScene() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}

	create<Model>(
		"./resources/2.fbx",
		ResourceStore::Get<ComPtr<ID3DBlob>>(VertexShader::BOX),
		ResourceStore::Get<ComPtr<ID3DBlob>>(PixelShader::BOX)
	).makeIndex(Key::FBX1);

	create<Debugger>();
}
