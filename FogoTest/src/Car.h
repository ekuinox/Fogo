#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <Fogo.h>
#include "Fogo/Game/ComponentInterface.h"
#include "Fogo/Graphics/DX12/DrawObject/FBXModel.h"

class Car : public Fogo::Game::ComponentInterface {
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	std::unique_ptr<Fogo::Graphics::DX12::DrawObject::FBXModel> __model;

	auto createRootSignature() -> void;
	auto createPipelineStateObject() -> void;
public:
	Car(const char * fileName);
	auto update() -> void override;
	auto render() const -> void override;
};