#include "Texture.h"
#include "../../Utility.h"
#include <DirectXTex.h>
#include "../DX12.h"

using namespace Fogo;

auto Texture::load(LPCWSTR filename, Type type) -> void {
	DirectX::TexMetadata metadata {};
	DirectX::ScratchImage scratch;
	if (type == Type::TGA) LoadFromTGAFile(filename, &metadata, scratch);
	else if (type == Type::ANY) LoadFromWICFile(filename, 0, &metadata, scratch, [&](IWICMetadataQueryReader * reader) { });

	//テクスチャ用のリソースの作成
	constexpr D3D12_HEAP_PROPERTIES textureHeapProperties {
		D3D12_HEAP_TYPE_CUSTOM,
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0,
		0,
		0
	};

	const D3D12_RESOURCE_DESC textureResourceDesc {
		static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension),
		0,
		static_cast<UINT64>(metadata.width),
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels),
		metadata.format,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
	};

	ExecOrFail<exception>(Graphics::GetDevice()->CreateCommittedResource(
		&textureHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&__resource)
	));

	//テクスチャ用のデスクリプタヒープの作成
	constexpr D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc {
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		0
	};

	ExecOrFail<exception>(Graphics::GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&__descriptor_heap)));

	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc {
		metadata.format,
		D3D12_SRV_DIMENSION_TEXTURE2D,
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
	};
	resourceViewDesc.Texture2D = D3D12_TEX2D_SRV { 0, 1, 0,	0.0F };

	Graphics::GetDevice()->CreateShaderResourceView(__resource.Get(), &resourceViewDesc, __descriptor_heap->GetCPUDescriptorHandleForHeapStart());

	//画像データの書き込み
	const auto & box = D3D12_BOX { 0, 0, 0, static_cast<UINT>(metadata.width), static_cast<UINT>(metadata.height), 1 };
	ExecOrFail<exception>(__resource->WriteToSubresource(0, &box, scratch.GetPixels(), sizeof(UINT) * metadata.width, sizeof(UINT) * metadata.width * metadata.height));
}

Texture::Texture(LPCWSTR filename, Type type) {
	load(filename, type);
}

auto Texture::getDescriptorHeap() const -> const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&
{
	return __descriptor_heap;
}
