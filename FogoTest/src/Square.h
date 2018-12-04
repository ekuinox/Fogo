#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <Fogo.h>
#include "Fogo/Graphics/DX12/Texture.h"

class Square
{
private:
	template <typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> constantBuffer;

	std::shared_ptr<Fogo::Graphics::DX12::Texture> texture;

	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;

	DirectX::XMMATRIX matrix;

	struct TexturedVertex3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	auto createRootSignature(ID3D12Device * device) -> void;
	auto createPipelineStateObject(ID3D12Device * device) -> void;

public:
	struct Option {
		DirectX::XMFLOAT2 center = { 0.0f, 0.0f }, size = { 1.0f, 1.0f };
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr float left() const { return center.x - size.x; }
		constexpr float right() const { return center.x + size.x; }
		constexpr float top() const { return center.y + size.y; }
		constexpr float bottom() const { return center.y - size.y; }
		static constexpr unsigned char vertexesCount = 4;
	};

	Square(ID3D12Device * device, std::shared_ptr<Fogo::Graphics::DX12::Texture> texture, const Option & option);
	auto update() -> void;
	auto render(ID3D12GraphicsCommandList * commandList) const -> void;
};
