#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12RootSignatureDescExtended : D3D12_ROOT_SIGNATURE_DESC {
		using Super = D3D12_ROOT_SIGNATURE_DESC;
		using This = D3D12RootSignatureDescExtended;
		constexpr This(const Super  & raw) : Super(raw) {}
		constexpr This() : Super() {}
		constexpr auto withNumParameters(const UINT & newNumParameters) const -> This;
		constexpr auto withParameters(const D3D12_ROOT_PARAMETER * newParameters) const -> This;
		constexpr auto withNumStaticSamplers(const UINT & newNumStaticSamplers) const -> This;
		constexpr auto withStaticSamplers(const D3D12_STATIC_SAMPLER_DESC * newStaticSamplers) const -> This;
		constexpr auto withFlags(const D3D12_ROOT_SIGNATURE_FLAGS & newFlags) const -> This;
	};

	constexpr auto D3D12RootSignatureDescExtended::withNumParameters(const UINT & newNumParameters) const -> This
	{
		return Super {
			newNumParameters,
			pParameters,
			NumStaticSamplers,
			pStaticSamplers,
			Flags
		};
	}

	constexpr auto D3D12RootSignatureDescExtended::withParameters(const D3D12_ROOT_PARAMETER * newParameters) const -> This
	{
		return Super {
			NumParameters,
			newParameters,
			NumStaticSamplers,
			pStaticSamplers,
			Flags
		};
	}

	constexpr auto D3D12RootSignatureDescExtended::withNumStaticSamplers(const UINT & newNumStaticSamplers) const -> This
	{
		return Super {
			NumParameters,
			pParameters,
			newNumStaticSamplers,
			pStaticSamplers,
			Flags
		};
	}

	constexpr auto D3D12RootSignatureDescExtended::withStaticSamplers(const D3D12_STATIC_SAMPLER_DESC * newStaticSamplers) const -> This
	{
		return Super {
			NumParameters,
			pParameters,
			NumStaticSamplers,
			newStaticSamplers,
			Flags
		};
	}

	constexpr auto D3D12RootSignatureDescExtended::withFlags(const D3D12_ROOT_SIGNATURE_FLAGS & newFlags) const -> This
	{
		return Super {
			NumParameters,
			pParameters,
			NumStaticSamplers,
			pStaticSamplers,
			newFlags
		};
	}

}
