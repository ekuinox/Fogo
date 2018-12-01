#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <vector>
#include <stdexcept>

namespace Fogo::Graphics::DX12 {
	class RenderTargetView {
	public:
		struct exception : std::exception { exception() : std::exception("[DX12RenderTargetView] error") { } };

	private:
		const unsigned char count;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heap;

		struct Target {
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			D3D12_CPU_DESCRIPTOR_HANDLE handle;
		};
		std::vector<Target> __targets;

	public:
		RenderTargetView(
			const Microsoft::WRL::ComPtr<ID3D12Device> & device,
			const Microsoft::WRL::ComPtr<IDXGISwapChain3> & swapChain,
			const unsigned char count
		);
		auto getTarget(const UINT index) const -> const Microsoft::WRL::ComPtr<ID3D12Resource>;
		auto getTargetHandle(const UINT index) const -> const D3D12_CPU_DESCRIPTOR_HANDLE;

	};
}
