#include "./Model.h"

using Fogo::Game::Meta;
using Fogo::Game::Initializer;
using Fogo::Game::Finalizer;
using Fogo::Game::Updater;
using Fogo::Game::Renderer;
using Microsoft::WRL::ComPtr;

void Model::initialize() {
	model = std::make_unique<Fogo::Graphics::DX12::FBXModel>(
		modelFile,
		Fogo::Graphics::DX12::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
		.setPixelShader(pixelShader).setVertexShader(vertexShader)
		);
	model->world = DirectX::XMMatrixIdentity();
}

void Model::update() {
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

	// コンスタントバッファ更新
	using namespace DirectX;

	model->world = rotation_matrix * translation_matrix * model->world;
	model->view = XMMatrixLookAtLH({ 0, 0, -50 }, { 0, 0, 0 }, { 0, 1, 0 });
	model->projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60),
		static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()),
		1,
		1000
	);
}

void Model::render() {
	model->render();
}

Model::Model(const char * modelFile, ComPtr<ID3DBlob> vertexShader, ComPtr<ID3DBlob> pixelShader)
	: modelFile(modelFile), vertexShader(vertexShader), pixelShader(pixelShader) {
	create<Initializer>([&] { initialize(); });
	create<Updater>([&] { update(); });
	create<Renderer>([&] { render(); });
	create<Meta>("Model", "Model", 0);
}
