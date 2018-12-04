#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <stdexcept>

namespace Fogo::Graphics::DX12 {
	class Texture {
	public:
		struct exception : std::exception { exception() : std::exception("[DX12RenderTargetView] error") { } };
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> __resource;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heap;

		auto load(const Microsoft::WRL::ComPtr<ID3D12Device> & device, LPCWSTR filename) -> void;
	public:
		Texture(const Microsoft::WRL::ComPtr<ID3D12Device> & device, LPCWSTR filename);
		auto getDescriptorHeap() const -> const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &;
	};
}