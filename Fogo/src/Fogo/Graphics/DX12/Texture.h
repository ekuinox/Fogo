#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <stdexcept>

namespace Fogo {
	class Texture {
	public:
		struct exception : std::exception { exception() : std::exception("[DX12Texture] error") { } };
		enum class Type {
			TGA,
			ANY
		};
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> __resource;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heap;

		auto load(LPCWSTR filename, Type type) -> void;
	public:
		Texture(LPCWSTR filename, Type type = Type::ANY);
		auto getDescriptorHeap() const -> const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &;
	};
}