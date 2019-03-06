#include "./Model.h"
#include "./Camera.h"
#include <DirectXMath.h>

using Fogo::Meta;
using Fogo::Initializer;
using Fogo::Finalizer;
using Fogo::Updater;
using Fogo::Renderer;
using Microsoft::WRL::ComPtr;

void Model::initialize() {
	model = std::make_unique<Fogo::FBXModel>(
		modelFile,
		Fogo::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
		.setPixelShader(pixelShader).setVertexShader(vertexShader)
		);
	model->world = DirectX::XMMatrixIdentity();
}

void Model::update() {
	using namespace DirectX;
	using namespace Fogo;

	static constexpr auto SPEED = 10.0f;

	XMFLOAT3 translation { 0, 0, 0 };
	XMFLOAT3 rotation { 0, 0, 0 };

	if (Input::GetPress(KeyCode::A)) rotation.y -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::D)) rotation.y += Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::W)) translation.z += Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::S)) translation.z -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::LShift)) translation.y -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::Space)) translation.y += Time::GetElapsedTime() * SPEED;

	using namespace DirectX;

	model->world
		= XMMatrixRotationX(Degree(rotation.x * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixRotationY(Degree(rotation.y * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixRotationZ(Degree(rotation.z * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixTranslation(translation.x, translation.y, translation.z)
		* model->world;

	if (const auto & scene = getMyScene()) {
		if (const auto & camera = scene->get<Camera>("MainCamera")) {
			model->view = camera->getView();
			model->projection = camera->getProjection();
		}
	}
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

DirectX::XMFLOAT3 Model::getPosition() const {
	using namespace DirectX;
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, model->world);
	return { mat._41, mat._42, mat._43 };
}
