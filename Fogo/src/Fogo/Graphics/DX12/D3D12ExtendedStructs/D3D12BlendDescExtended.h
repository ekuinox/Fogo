#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12BlendDescExtended : D3D12_BLEND_DESC {
		using Super = D3D12_BLEND_DESC;
		using This = D3D12BlendDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		
		constexpr auto withAlphaToCoverageEnable(const BOOL & value) const -> This {
			return Super {
				value,
				IndependentBlendEnable,
				{
					RenderTarget[0],
					RenderTarget[1],
					RenderTarget[2],
					RenderTarget[3],
					RenderTarget[4],
					RenderTarget[5],
					RenderTarget[6],
					RenderTarget[7],
				}
			};
		}
		
		constexpr auto withIndependentBlendEnable(const BOOL & value) const -> This {
			return Super {
				AlphaToCoverageEnable,
				value,
				{
					RenderTarget[0],
					RenderTarget[1],
					RenderTarget[2],
					RenderTarget[3],
					RenderTarget[4],
					RenderTarget[5],
					RenderTarget[6],
					RenderTarget[7],
				}
			};
		}
		
		constexpr auto withRenderTarget(const decltype(RenderTarget) & value) const -> This {
			return Super {
				AlphaToCoverageEnable,
				IndependentBlendEnable,
				{
					value[0],
					value[1],
					value[2],
					value[3],
					value[4],
					value[5],
					value[6],
					value[7],
				}
			};
		}
		
		constexpr auto withRenderTarget0(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				value,
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7],
			});
		}

		
		constexpr auto withRenderTarget1(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				value,
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget2(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				value,
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget3(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				RenderTarget[2],
				value,
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget4(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				value,
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget5(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				value,
				RenderTarget[6],
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget6(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				value,
				RenderTarget[7],
			});
		}
		
		constexpr auto withRenderTarget7(const D3D12_RENDER_TARGET_BLEND_DESC & value) const -> This {
			return withRenderTarget({
				RenderTarget[0],
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				value,
			});
		}
	};

}