#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12RootDescriptorTableExtended : D3D12_ROOT_DESCRIPTOR_TABLE {
		using Super = D3D12_ROOT_DESCRIPTOR_TABLE;
		using This = D3D12RootDescriptorTableExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}

		constexpr auto withNumDescriptorRanges(const UINT & newNumDescriptorRanges) const -> This {
			return Super {
				newNumDescriptorRanges,
				this->pDescriptorRanges
			};
		}

		constexpr auto withDescriptorRanges(const D3D12_DESCRIPTOR_RANGE * newDescriptorRanges) const -> This {
			return Super {
				this->NumDescriptorRanges,
				newDescriptorRanges
			};
		}
	};

}
