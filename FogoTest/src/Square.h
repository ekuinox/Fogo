#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <stdexcept>
#include <DirectXMath.h>
#include <fstream>

class Square
{
private:
	template <typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> constantBuffer;

	ComPtr<ID3D12Resource> texture;
	ComPtr<ID3D12DescriptorHeap> textureDescriptorHeap;

	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;

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
		constexpr float buttom() const { return center.y - size.y; }
		static constexpr unsigned char vertexesCount = 4;
	};

	Square(ID3D12Device * device, const Option & option);
	auto render(ID3D12GraphicsCommandList * commandList) const -> void;
};