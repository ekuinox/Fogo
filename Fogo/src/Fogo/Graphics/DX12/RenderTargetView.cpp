#include "RenderTargetView.h"
#include "../../Utility/HelperFunctions.h"

using namespace Microsoft::WRL;
using namespace Fogo::Graphics::DX12;
using namespace Fogo::Utility;

RenderTargetView::RenderTargetView(
	const ComPtr<ID3D12Device> & device,
	const ComPtr<IDXGISwapChain3> & swapChain,
	const unsigned char count
) : count(count) {
	const D3D12_DESCRIPTOR_HEAP_DESC heapDesc {
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		count,
		D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	ExecOrFail<exception>(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(__descriptor_heap.GetAddressOf())));
	
	const auto size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	__targets.resize(count);
	
	for (auto i = 0; i < count; ++i) {
		ExecOrFail<exception>(swapChain->GetBuffer(i, IID_PPV_ARGS(&__targets[i].resource)));
		
		__targets[i].handle = __descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		__targets[i].handle.ptr += size * i;
		
		device->CreateRenderTargetView(__targets[i].resource.Get(), nullptr, __targets[i].handle);
	}
}

auto RenderTargetView::getTarget(const UINT index) const -> const Microsoft::WRL::ComPtr<ID3D12Resource> {
	return __targets.at(index).resource;
}

auto RenderTargetView::getTargetHandle(const UINT index) const -> const D3D12_CPU_DESCRIPTOR_HANDLE {
	return __targets.at(index).handle;
}
