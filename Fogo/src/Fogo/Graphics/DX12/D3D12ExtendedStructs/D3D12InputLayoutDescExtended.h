#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12InputLayoutDescExtended : D3D12_INPUT_LAYOUT_DESC {
		using Super = D3D12_INPUT_LAYOUT_DESC;
		using This = D3D12InputLayoutDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}

		constexpr auto withInputElementDescs(const D3D12_INPUT_ELEMENT_DESC * value) const -> This {
			return Super {
				value,
				NumElements
			};
		}

		constexpr auto withNumElements(const UINT & value) const -> This {
			return Super {
				pInputElementDescs,
				value
			};
		}

		template <size_t SIZE>
		static constexpr auto FromInputElementDescs(const D3D12_INPUT_ELEMENT_DESC value[SIZE]) -> This {
			return Super {
				value,
				SIZE
			};
		}
			
	};

}
