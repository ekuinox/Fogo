#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <Fogo.h>

class Square : public Fogo::Game::Component, Fogo::Game::LifeCycled {
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	std::unique_ptr<Fogo::Graphics::DX12::DrawObject::Plain> __plain;


	auto createRootSignature() -> void;
	auto createPipelineStateObject() -> void;

public:
	struct Option {
		std::shared_ptr<Fogo::Graphics::DX12::Texture> texture;
		DirectX::XMFLOAT2 center = { 0.0f, 0.0f }, size = { 1.0f, 1.0f };
	};

	Square(const Option & option);
};
