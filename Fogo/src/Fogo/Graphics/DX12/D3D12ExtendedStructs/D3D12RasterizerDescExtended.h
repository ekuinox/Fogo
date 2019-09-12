#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12RasterizerDescExtended : D3D12_RASTERIZER_DESC {
		using Super = D3D12_RASTERIZER_DESC;
		using This = D3D12RasterizerDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		
		constexpr auto withFillMode(const D3D12_FILL_MODE & value) const -> This {
			return Super {
				value,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withCullMode(const D3D12_CULL_MODE & value) const -> This {
			return Super {
				FillMode,
				value,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withFrontCounterClockwise(const BOOL & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				value,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withDepthBias(const INT & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				value,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withDepthBiasClamp(const FLOAT & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				value,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withSlopeScaledDepthBias(const FLOAT & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				value,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withDepthClipEnable(const BOOL & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				value,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withMultisampleEnable(const BOOL & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				value,
				AntialiasedLineEnable,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withAntialiasedLineEnable(const BOOL & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				value,
				ForcedSampleCount,
				ConservativeRaster
			};
		}
		
		constexpr auto withForcedSampleCount(const UINT & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				value,
				ConservativeRaster
			};
		}
		
		constexpr auto withConservativeRaster(const D3D12_CONSERVATIVE_RASTERIZATION_MODE & value) const -> This {
			return Super {
				FillMode,
				CullMode,
				FrontCounterClockwise,
				DepthBias,
				DepthBiasClamp,
				SlopeScaledDepthBias,
				DepthClipEnable,
				MultisampleEnable,
				AntialiasedLineEnable,
				ForcedSampleCount,
				value
			};
		}
	};

}
