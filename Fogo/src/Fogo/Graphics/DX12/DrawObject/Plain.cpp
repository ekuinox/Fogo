#include "Plain.h"

#include <utility>
#include <vector>
#include <d3dcompiler.h>
#include "../Graphics.h"
#include "../D3D12ExtendedStructs.h"
#include "../DXGIExtendedStructs.h"
#include "../../../Utility/HelperFunctions.h"
#include "../../../Utility/Time.h"

using namespace Fogo;

Plain::Plain(
	const Microsoft::WRL::ComPtr<ID3D12Device> & device,
	std::shared_ptr<Texture> texture,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature
) : __vertex_buffer({}),
	__constant_buffer({}),
	__texture(std::move(texture)),
	__pipeline_state(std::move(pipelineState)),
	__root_signature(std::move(rootSignature))
{
	static constexpr D3D12_HEAP_PROPERTIES HEAP_PROPERTIES = D3D12HeapPropertiesExtended().withType(D3D12_HEAP_TYPE_UPLOAD);

	static constexpr D3D12_RESOURCE_DESC RESOURCE_DESC = D3D12ResourceDescExtended()
		.withDimension(D3D12_RESOURCE_DIMENSION_BUFFER)
		.withAlignment(0)
		.withWidth(256)
		.withHeight(1)
		.withDepthOrArraySize(1)
		.withMipLevels(1)
		.withFormat(DXGI_FORMAT_UNKNOWN)
		.withSampleDesc(DXGISampleDescExtended().withCount(1).withQuality(0))
		.withLayout(D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

	ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&__vertex_buffer)
	));

	ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&__constant_buffer)
	));

	TexturedVertex3D * buffer{ };

	ExecOrFail(__vertex_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer)));

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

	matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Time::GetElapsedTime()));
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
