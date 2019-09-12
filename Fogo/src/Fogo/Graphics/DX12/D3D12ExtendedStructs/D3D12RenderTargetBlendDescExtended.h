#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12RenderTargetBlendDescExtended : D3D12_RENDER_TARGET_BLEND_DESC {
		using Super = D3D12_RENDER_TARGET_BLEND_DESC;
		using This = D3D12RenderTargetBlendDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		
		constexpr auto withBlendEnable(const BOOL & value) const -> This {
			return Super {
				value,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withLogicOpEnable(const BOOL & value) const -> This {
			return Super {
				BlendEnable,
				value,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withSrcBlend(const D3D12_BLEND & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				value,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withDestBlend(const D3D12_BLEND & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				value,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withBlendOp(const D3D12_BLEND_OP & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				value,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withSrcBlendAlpha(const D3D12_BLEND & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				value,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withDestBlendAlpha(const D3D12_BLEND & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				value,
				BlendOpAlpha,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withBlendOpAlpha(const D3D12_BLEND_OP & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				value,
				LogicOp,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withLogicOp(const D3D12_LOGIC_OP & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				value,
				RenderTargetWriteMask,
			};
		}
		
		constexpr auto withRenderTargetWriteMask(const UINT8 & value) const -> This {
			return Super {
				BlendEnable,
				LogicOpEnable,
				SrcBlend,
				DestBlend,
				BlendOp,
				SrcBlendAlpha,
				DestBlendAlpha,
				BlendOpAlpha,
				LogicOp,
				value,
			};
		}
	};

}
