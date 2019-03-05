#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <memory>
#include "../Texture.h"
#include <DirectXMath.h>

namespace Fogo {
	class Plain
	{
	public:
		DirectX::XMMATRIX matrix{};
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> __vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> __constant_buffer;
		std::shared_ptr<Texture> __texture;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> __pipeline_state;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> __root_signature;

		struct TexturedVertex3D
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
		};

	public:
		struct Option {
			static constexpr DirectX::XMFLOAT2 CENTER = { 0.0f, 0.0f }, SIZE = { 1.0f, 1.0f };
			static constexpr float LEFT = CENTER.x - SIZE.x;
			static constexpr float RIGHT = CENTER.x + SIZE.x;
			static constexpr float TOP = CENTER.y + SIZE.y;
			static constexpr float BOTTOM = CENTER.y - SIZE.y;
			static constexpr unsigned char VERTEXES_COUNT = 4;
		};

		Plain(
			const Microsoft::WRL::ComPtr<ID3D12Device> & device,
			std::shared_ptr<Texture> texture,
			Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
			Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature
		);
		auto update() -> void;
		auto render(
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const DirectX::XMMATRIX & view,
			const DirectX::XMMATRIX & projection
		) const -> void;
	};

}
