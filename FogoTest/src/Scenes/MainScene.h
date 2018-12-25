#pragma once

#include <Fogo.h>
#include "../Square.h"
#include "../FBXSample.h"
#include <iostream>

class MainScene : public Fogo::Game::Scene {
private:
	struct InputDebugger : ComponentInterface {
		// nothing
	};

	struct FBX : ComponentInterface {
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
		const char * modelFile;
		std::unique_ptr<Fogo::Graphics::DX12::FBXModel> model;
		void initialize() override {
			static auto once = true;
			if (once) {
				once = false;
				model = std::make_unique<Fogo::Graphics::DX12::FBXModel>(
					modelFile,
					Fogo::Graphics::DX12::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
					.setPixelShader(pixelShader).setVertexShader(vertexShader)
				);
			}
			model->matrix = DirectX::XMMatrixIdentity();
			ComponentInterface::initialize();
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
			ComponentInterface::render();
		}
	};

public:
	using ComponentStore = Fogo::Utility::TreeStore<
		std::shared_ptr<ComponentInterface>, int,
		std::shared_ptr<Square>, std::shared_ptr<FBXSample>, std::shared_ptr<InputDebugger>, std::shared_ptr<FBX>>;
	enum class VertexShader { BOX };
	enum class PixelShader { BOX };
	enum class TextureType { BOX };
	using ResourceStore = Fogo::Utility::MappedStore<
		VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		TextureType, std::shared_ptr<Fogo::Graphics::DX12::Texture>
	>;

	void initialize() override;
	void update() override;
	void finalize() override;
};
