#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <Fogo.h>
#include "Fogo/Game/ComponentInterface.h"

class Square : public Fogo::Game::ComponentInterface
{
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	std::unique_ptr<Fogo::Graphics::DX12::DrawObject::Plain> __plain;


	auto createRootSignature(ID3D12Device * device) -> void;
	auto createPipelineStateObject(ID3D12Device * device) -> void;

public:
	struct Option {
		DirectX::XMFLOAT2 center = { 0.0f, 0.0f }, size = { 1.0f, 1.0f };
		std::shared_ptr<Fogo::Graphics::DX12::Texture> texture;
	};

	Square(ID3D12Device * device, const Option & option);
	auto update() -> void override;
	auto render() const -> void override;
};
