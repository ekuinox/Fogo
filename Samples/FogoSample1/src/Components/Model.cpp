#include "./Model.h"
#include "./Camera.h"
#include <DirectXMath.h>
#include "CameraController.h"

using namespace DirectX;
using namespace Fogo;
using Microsoft::WRL::ComPtr;

void Model::initialize() {
	model = std::make_unique<Fogo::FBXModel>(
		modelFile,
		Fogo::FBXModel::Properties().setTextureDirectory(L"./resources/Textures/")
		.setPixelShader(pixelShader).setVertexShader(vertexShader)
		);
	model->world = XMMatrixIdentity();
}

void Model::render() const {
	model->world
		= XMMatrixRotationX(Degree(rotation.x * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixRotationY(Degree(rotation.y * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixRotationZ(Degree(rotation.z * Time::GetElapsedTime() * 360).toRadian())
		* XMMatrixTranslation(translation.x, translation.y, translation.z)
		* model->world;

	const auto & cameraController = getMyScene()->get<CameraController>();
	if (!cameraController) return;

	const auto & camera = cameraController->getCurrentCamera();

	model->view = camera->getView();
	model->projection = camera->getProjection();

	model->render();
}

Model::Model(const char * modelFile, ComPtr<ID3DBlob> vertexShader, ComPtr<ID3DBlob> pixelShader)
	: modelFile(modelFile), vertexShader(vertexShader), pixelShader(pixelShader), translation({ 0, 0, 0 }), rotation({ 0, 0, 0 }) {
	create<Initializer>([&] { initialize(); });
	create<Renderer>([&] { render(); });
}

DirectX::XMFLOAT3 Model::getPosition() const {
	using namespace DirectX;
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, model->world);
	return { mat._41, mat._42, mat._43 };
}

void Model::setPosition(XMFLOAT3 position) const {
	model->world = XMMatrixTranslation(position.x, position.y, position.z) * model->world;
}
