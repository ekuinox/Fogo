#include "DepthStencilView.h"
#include "../../Utility/HelperFunctions.h"

using namespace Fogo::Graphics::DX12;
using namespace Fogo::Utility;

DepthStencilView::DepthStencilView(const Microsoft::WRL::ComPtr<ID3D12Device> & device, const UINT width, const UINT height) {
	static constexpr D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc {
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	ExecOrFail<exception>(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&__descriptor_heap)));

	static constexpr D3D12_HEAP_PROPERTIES heapProperties {
		D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	const D3D12_RESOURCE_DESC resourceDesc {
		D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		width,
		height,
		1,
		0,
		DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	D3D12_CLEAR_VALUE clearValue {
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
	};
	clearValue.DepthStencil = D3D12_DEPTH_STENCIL_VALUE{ 1.0f, 0 };

	ExecOrFail<exception>(device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(__buffer.GetAddressOf())
	));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc {
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
		D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D,
		D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE
	};
	dsvDesc.Texture2D.MipSlice = 0;

	__handle = __descriptor_heap->GetCPUDescriptorHandleForHeapStart();

	device->CreateDepthStencilView(__buffer.Get(), &dsvDesc, __handle);
}

auto DepthStencilView::getHandle() const -> const D3D12_CPU_DESCRIPTOR_HANDLE
{
	return __handle;
}
