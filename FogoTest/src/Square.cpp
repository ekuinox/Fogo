#include "Square.h"
#include <utility>
#include <vector>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

auto Square::createRootSignature(ID3D12Device * device) -> void {
	static constexpr D3D12_DESCRIPTOR_RANGE range[1] = { D3D12_DESCRIPTOR_RANGE {
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		1,
		0,
		0,
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	} };

	D3D12_ROOT_PARAMETER rootParameters[2];

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor = D3D12_ROOT_DESCRIPTOR{ 0, 0 };

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable = D3D12_ROOT_DESCRIPTOR_TABLE{ 1, &range[0] };

	static constexpr D3D12_STATIC_SAMPLER_DESC SAMPLER_DESC {
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		0.0f,
		16,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
		0.0f,
		D3D12_FLOAT32_MAX,
		0,
		0,
		D3D12_SHADER_VISIBILITY_ALL
	};

	const auto & rootSignatureDesc = D3D12_ROOT_SIGNATURE_DESC {
		_countof(rootParameters),
		rootParameters,
		1,
		&SAMPLER_DESC,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};

	ComPtr<ID3DBlob> blob;

	Fogo::Utility::ExecOrFail(D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob,
		nullptr
	));

	Fogo::Utility::ExecOrFail(device->CreateRootSignature(
		0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	));
}

auto Square::createPipelineStateObject(ID3D12Device * device) -> void {
	const auto & vertexShader = Fogo::Graphics::DX12::Graphics::CompileVertexShader(L"resources/shaders.hlsl");
	const auto & pixelShader = Fogo::Graphics::DX12::Graphics::CompilePixelShader(L"resources/shaders.hlsl");

	constexpr D3D12_INPUT_ELEMENT_DESC inputElementDesc[3] {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	const D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc {
		rootSignature.Get(),
		D3D12_SHADER_BYTECODE { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() },
		D3D12_SHADER_BYTECODE { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() },
		D3D12_SHADER_BYTECODE { },
		D3D12_SHADER_BYTECODE { },
		D3D12_SHADER_BYTECODE { },
		D3D12_STREAM_OUTPUT_DESC { },
		D3D12_BLEND_DESC {
			FALSE,
			FALSE,
			{ D3D12_RENDER_TARGET_BLEND_DESC {
				FALSE,
				FALSE,
				D3D12_BLEND_ONE,
				D3D12_BLEND_ZERO,
				D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE,
				D3D12_BLEND_ZERO,
				D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_CLEAR,
				D3D12_COLOR_WRITE_ENABLE_ALL
			}}
		},
		UINT_MAX,
		D3D12_RASTERIZER_DESC {
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_NONE,
			FALSE,
			0,
			0,
			0,
			TRUE,
			FALSE,
			FALSE,
			FALSE,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		},
		D3D12_DEPTH_STENCIL_DESC {
			TRUE,
			D3D12_DEPTH_WRITE_MASK_ALL,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			FALSE,
			D3D12_DEFAULT_STENCIL_READ_MASK,
			D3D12_DEFAULT_STENCIL_WRITE_MASK,
			D3D12_DEPTH_STENCILOP_DESC {
				D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC_ALWAYS
			},
			D3D12_DEPTH_STENCILOP_DESC {
				D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC_ALWAYS
			}
		},
		D3D12_INPUT_LAYOUT_DESC { inputElementDesc,	_countof(inputElementDesc) },
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		1,
		{ DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM },
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
		DXGI_SAMPLE_DESC { 1, 0 },
		0,
		D3D12_CACHED_PIPELINE_STATE { },
		D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE
	};

	Fogo::Utility::ExecOrFail(device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pipelineState)));
}

Square::Square(ID3D12Device * device, const Option & option) {
	createRootSignature(device);
	createPipelineStateObject(device);

	__plain = std::make_unique<Fogo::Graphics::DX12::DrawObject::Plain>(device, option.texture, pipelineState, rootSignature);
	__plain->matrix = XMMatrixIdentity();
	__plain->matrix *= XMMatrixScaling(option.size.x, option.size.y, 0);
	__plain->matrix *= XMMatrixTranslation(option.center.x, option.center.y, 0);
}

auto Square::update() const -> void {

	__plain->matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Fogo::Utility::Time::GetElapsedTime()));
}

auto Square::render(ComPtr<ID3D12GraphicsCommandList> commandList) const -> void {
	__plain->render(
		commandList,
		XMMatrixLookAtLH({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }),
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 640.0f / 480.0f, 1.0f, 20.0f)
	);
}
