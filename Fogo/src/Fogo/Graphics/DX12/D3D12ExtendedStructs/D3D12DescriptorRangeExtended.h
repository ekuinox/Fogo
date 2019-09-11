#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {

	struct D3D12DescriptorRangeExtended : D3D12_DESCRIPTOR_RANGE {
		using Super = D3D12_DESCRIPTOR_RANGE;
		using This = D3D12DescriptorRangeExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		constexpr auto withRangeType(const D3D12_DESCRIPTOR_RANGE_TYPE & newRangeType) const -> This;
		constexpr auto withNumDescriptors(const UINT & newNumDescriptors) const -> This;
		constexpr auto withBaseShaderRegister(const UINT & newBaseShaderRegister) const -> This;
		constexpr auto withRegisterSpace(const UINT & newRegisterSpace) const -> This;
		constexpr auto withOffsetInDescriptorsFromTableStart(const UINT & newOffsetInDescriptorsFromTableStart) const -> This;
	};

	constexpr auto D3D12DescriptorRangeExtended::withRangeType(const D3D12_DESCRIPTOR_RANGE_TYPE & newRangeType) const -> This {
		return Super {
			newRangeType,
			this->NumDescriptors,
			this->BaseShaderRegister,
			this->RegisterSpace,
			this->OffsetInDescriptorsFromTableStart
		};
	}

	constexpr auto D3D12DescriptorRangeExtended::withNumDescriptors(const UINT & newNumDescriptors) const -> This {
		return Super {
			this->RangeType,
			newNumDescriptors,
			this->BaseShaderRegister,
			this->RegisterSpace,
			this->OffsetInDescriptorsFromTableStart
		};
	}

	constexpr auto D3D12DescriptorRangeExtended::withBaseShaderRegister(const UINT & newBaseShaderRegister) const -> This {
		return Super {
			this->RangeType,
			this->NumDescriptors,
			newBaseShaderRegister,
			this->RegisterSpace,
			this->OffsetInDescriptorsFromTableStart
		};
	}

	constexpr auto D3D12DescriptorRangeExtended::withRegisterSpace(const UINT & newRegisterSpace) const -> This {
		return Super {
			this->RangeType,
			this->NumDescriptors,
			this->BaseShaderRegister,
			newRegisterSpace,
			this->OffsetInDescriptorsFromTableStart
		};
	}

	constexpr auto D3D12DescriptorRangeExtended::withOffsetInDescriptorsFromTableStart(const UINT & newOffsetInDescriptorsFromTableStart) const -> This {
		return Super {
			this->RangeType,
			this->NumDescriptors,
			this->BaseShaderRegister,
			this->RegisterSpace,
			newOffsetInDescriptorsFromTableStart
		};
	}

}
