#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12GraphicsPipelineStateDescExtended : D3D12_GRAPHICS_PIPELINE_STATE_DESC {
		using Super = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
		using This = D3D12GraphicsPipelineStateDescExtended;
		constexpr This(const Super& raw) : Super(raw) { }
		constexpr This() : Super() { }
		
		constexpr auto withRootSignature(ID3D12RootSignature * value) const -> This {
			return Super {
				value,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withVS(const D3D12_SHADER_BYTECODE & value) const -> This {
			return Super {
				pRootSignature,
				value,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withPS(const D3D12_SHADER_BYTECODE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				value,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withDS(const D3D12_SHADER_BYTECODE & value) const -> This {
			return Super{
				pRootSignature,
				VS,
				PS,
				value,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withHS(const D3D12_SHADER_BYTECODE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				value,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withGS(const D3D12_SHADER_BYTECODE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				value,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withStreamOutput(const D3D12_STREAM_OUTPUT_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				value,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withBlendState(const D3D12_BLEND_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				value,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withSampleMask(const UINT & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				value,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withRasterizerState(const D3D12_RASTERIZER_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				value,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withDepthStencilState(const D3D12_DEPTH_STENCIL_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				value,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withInputLayout(const D3D12_INPUT_LAYOUT_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				value,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withIBStripCutValue(const D3D12_INDEX_BUFFER_STRIP_CUT_VALUE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				value,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY_TYPE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				value,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withNumRenderTargets(const UINT & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				value,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withRTVFormats(const decltype(RTVFormats) & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					value[0],
					value[1],
					value[2],
					value[3],
					value[4],
					value[5],
					value[6],
					value[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withRTVFormat0(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				value,
				RTVFormats[1],
				RTVFormats[2],
				RTVFormats[3],
				RTVFormats[4],
				RTVFormats[5],
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat1(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				value,
				RTVFormats[2],
				RTVFormats[3],
				RTVFormats[4],
				RTVFormats[5],
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat2(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				value,
				RTVFormats[3],
				RTVFormats[4],
				RTVFormats[5],
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat3(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				RTVFormats[2],
				value,
				RTVFormats[4],
				RTVFormats[5],
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat4(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				RTVFormats[2],
				RTVFormats[3],
				value,
				RTVFormats[5],
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat5(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				RTVFormats[2],
				RTVFormats[3],
				RTVFormats[4],
				value,
				RTVFormats[6],
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat6(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				RTVFormats[2],
				RTVFormats[3],
				RTVFormats[4],
				RTVFormats[5],
				value,
				RTVFormats[7],
			});
		}

		constexpr auto withRTVFormat7(const DXGI_FORMAT & value) const -> This {
			return withRTVFormats({
				RTVFormats[0],
				RTVFormats[1],
				RTVFormats[2],
				RTVFormats[3],
				RTVFormats[4],
				RTVFormats[5],
				RTVFormats[6],
				value,
			});
		}

		constexpr auto withDSVFormat(const DXGI_FORMAT & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				value,
				SampleDesc,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withSampleDesc(const DXGI_SAMPLE_DESC & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				value,
				NodeMask,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withNodeMask(const UINT & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				value,
				CachedPSO,
				Flags
			};
		}

		constexpr auto withCachedPSO(const D3D12_CACHED_PIPELINE_STATE & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				value,
				Flags
			};
		}

		constexpr auto withFlags(const D3D12_PIPELINE_STATE_FLAGS & value) const -> This {
			return Super {
				pRootSignature,
				VS,
				PS,
				DS,
				HS,
				GS,
				StreamOutput,
				BlendState,
				SampleMask,
				RasterizerState,
				DepthStencilState,
				InputLayout,
				IBStripCutValue,
				PrimitiveTopologyType,
				NumRenderTargets,
				{
					RTVFormats[0],
					RTVFormats[1],
					RTVFormats[2],
					RTVFormats[3],
					RTVFormats[4],
					RTVFormats[5],
					RTVFormats[6],
					RTVFormats[7],
				},
				DSVFormat,
				SampleDesc,
				NodeMask,
				CachedPSO,
				value
			};
		}

	};

}
