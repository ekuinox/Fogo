#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <stdexcept>

namespace Fogo::Graphics::DX12 {
	class Texture {
	public:
		struct exception : std::exception { exception() : std::exception("[DX12Texture] error") { } };
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> __resource;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heap;

		auto load(LPCWSTR filename) -> void;
	public:
		Texture(LPCWSTR filename);
		auto getDescriptorHeap() const -> const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &;
	};
}