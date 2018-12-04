#include "Texture.h"
#include "../../Utility.h"

using namespace Fogo::Graphics::DX12;
using namespace Fogo::Utility;

auto Texture::load(const Microsoft::WRL::ComPtr<ID3D12Device> & device, const char * filename) -> void {
	std::vector<byte> image;
	unsigned width, height;

	const auto a = lodepng::decode(image, width, height, filename);

	//テクスチャ用のリソースの作成
	constexpr D3D12_HEAP_PROPERTIES textureHeapProperties {
		D3D12_HEAP_TYPE_CUSTOM,
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0,
		0,
		0
	};

	const D3D12_RESOURCE_DESC textureResourceDesc {
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		static_cast<UINT64>(width),
		static_cast<UINT64>(height),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT_UNKNOWN
	};

	ExecOrFail<exception>(device->CreateCommittedResource(
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

	ExecOrFail<exception>(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&__descriptor_heap)));

	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_SRV_DIMENSION_TEXTURE2D,
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
	};
	resourceViewDesc.Texture2D = D3D12_TEX2D_SRV { 0, 1, 0,	0.0F };

	device->CreateShaderResourceView(__resource.Get(), &resourceViewDesc, __descriptor_heap->GetCPUDescriptorHandleForHeapStart());

	//画像データの書き込み
	const auto & box = D3D12_BOX{ 0, 0, 0, static_cast<UINT>(width), static_cast<UINT>(height), 1 };
	ExecOrFail<exception>(__resource->WriteToSubresource(0, &box, image.data(), 4 * width, 4 * width * height));
}

Texture::Texture(const Microsoft::WRL::ComPtr<ID3D12Device> & device, const char* filename) {
	load(device, filename);
}

auto Texture::getDescriptorHeap() const -> const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&
{
	return __descriptor_heap;
}
