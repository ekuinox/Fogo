#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <vector>
#include <stdexcept>

namespace Fogo::Graphics::DX12 {
	class DepthStencilView {
	public:
		struct exception : std::exception { exception() : std::exception("[DepthBuffer] error") { } };
	
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> __buffer;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE __handle;

	public:
		DepthStencilView(const Microsoft::WRL::ComPtr<ID3D12Device> & device, const UINT width, const UINT height);
		auto getHandle() const -> const D3D12_CPU_DESCRIPTOR_HANDLE;
	};
}