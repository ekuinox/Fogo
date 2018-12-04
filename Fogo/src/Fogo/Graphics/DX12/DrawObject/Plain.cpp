#include "Plain.h"

#include <utility>
#include <vector>
#include <d3dcompiler.h>
#include "../Graphics.h"
#include "../../../Utility/Exception.h"
#include "../../../Utility/Time.h"

using namespace Fogo::Graphics::DX12::DrawObject;

Plain::Plain(
	const Microsoft::WRL::ComPtr<ID3D12Device> & device,
	std::shared_ptr<Fogo::Graphics::DX12::Texture> texture,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature
) : __vertex_buffer({}),
	__constant_buffer({}),
	__texture(std::move(texture)),
	__pipeline_state(std::move(pipelineState)),
	__root_signature(std::move(rootSignature))
{
	static constexpr D3D12_HEAP_PROPERTIES HEAP_PROPERTIES {
		D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	static constexpr D3D12_RESOURCE_DESC RESOURCE_DESC {
		D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		256,
		1,
		1,
		1,
		DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR
	};

	Fogo::Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&__vertex_buffer)
	));

	Fogo::Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&__constant_buffer)
	));

	TexturedVertex3D * buffer{ };

	Utility::ExecOrFail(__vertex_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer)));

	buffer[0].position = { Option::LEFT, Option::TOP, 0.0f };
	buffer[1].position = { Option::RIGHT, Option::TOP, 0.0f };
	buffer[2].position = { Option::LEFT, Option::BOTTOM, 0.0f };
	buffer[3].position = { Option::RIGHT, Option::BOTTOM, 0.0f };

	for (auto i = 0; i < Option::VERTEXES_COUNT; ++i) buffer[i].normal = { 0.0f, 0.0f, 1.0f };

	buffer[0].uv = { 0.0f, 0.0f };
	buffer[1].uv = { 1.0f, 0.0f };
	buffer[2].uv = { 0.0f, 1.0f };
	buffer[3].uv = { 1.0f, 1.0f };

	__vertex_buffer->Unmap(0, nullptr);
	buffer = nullptr;

	matrix = DirectX::XMMatrixIdentity();
}

auto Plain::update() -> void {
	using namespace DirectX;

	matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Fogo::Utility::Time::GetElapsedTime()));
}

auto Plain::render(
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
	const DirectX::XMMATRIX & view,
	const DirectX::XMMATRIX & projection
) const -> void {
	using namespace DirectX;

	//ルートシグネチャとPSOの設定
	commandList->SetGraphicsRootSignature(__root_signature.Get());
	commandList->SetPipelineState(__pipeline_state.Get());

	XMFLOAT4X4 out {};
	XMStoreFloat4x4(&out, XMMatrixTranspose(matrix * view * projection));

	XMFLOAT4X4 * buffer{ };

	if (FAILED(__constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer)))) return;

	//行列を定数バッファに書き込み
	*buffer = out;

	__constant_buffer->Unmap(0, nullptr);
	buffer = nullptr;

	const auto & vertexView = D3D12_VERTEX_BUFFER_VIEW{
		__vertex_buffer->GetGPUVirtualAddress(),
		sizeof(TexturedVertex3D) * Option::VERTEXES_COUNT,
		sizeof(TexturedVertex3D)
	};

	//定数バッファをシェーダのレジスタにセット
	commandList->SetGraphicsRootConstantBufferView(0, __constant_buffer->GetGPUVirtualAddress());

	//テクスチャをシェーダのレジスタにセット
	ID3D12DescriptorHeap * heaps[] = { __texture->getDescriptorHeap().Get() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	commandList->SetGraphicsRootDescriptorTable(1, __texture->getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	//インデックスを使用しないトライアングルストリップで描画
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandList->IASetVertexBuffers(0, 1, &vertexView);

	//描画
	commandList->DrawInstanced(Option::VERTEXES_COUNT, 1, 0, 0);
}
