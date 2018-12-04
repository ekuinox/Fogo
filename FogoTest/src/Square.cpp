#include "Square.h"
#include <utility>
#include <vector>
#include <d3dcompiler.h>

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
	static constexpr UINT compileFlag = 0;

	ComPtr<ID3DBlob> vertexShader;
	Fogo::Utility::ExecOrFail(D3DCompileFromFile(
		L"resources/shaders.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		compileFlag,
		0,
		vertexShader.GetAddressOf(),
		nullptr
	));

	ComPtr<ID3DBlob> pixelShader;
	Fogo::Utility::ExecOrFail(D3DCompileFromFile(
		L"resources/shaders.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		compileFlag,
		0,
		pixelShader.GetAddressOf(),
		nullptr
	));

	constexpr D3D12_INPUT_ELEMENT_DESC inputElementDesc[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	const auto & pipelineStateDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC {
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

Square::Square(ID3D12Device * device, std::shared_ptr<Fogo::Graphics::DX12::Texture> texture, const Option & option)
	: vertexBuffer({}), constantBuffer({}), texture(std::move(texture))
{
	createRootSignature(device);
	createPipelineStateObject(device);

	static constexpr D3D12_HEAP_PROPERTIES HEAP_PROPERTIES {
		D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	static constexpr D3D12_RESOURCE_DESC RESOURCE_DESC {
		D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		256,
		1,
		1,
		1,
		DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR
	};

	Fogo::Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)
	));

	Fogo::Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer)
	));

	TexturedVertex3D * buffer{ };

	if (FAILED(vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer))))
	{
		throw std::exception("[PlainSquare] error");
	}

	buffer[0].position = { option.left(), option.top(), 0.0f };
	buffer[1].position = { option.right(), option.top(), 0.0f };
	buffer[2].position = { option.left(), option.bottom(), 0.0f };
	buffer[3].position = { option.right(), option.bottom(), 0.0f };

	for (auto i = 0; i < Option::vertexesCount; ++i)
	{
		buffer[i].normal = { 0.0f, 0.0f, 1.0f };
	}

	buffer[0].uv = { 0.0f, 0.0f };
	buffer[1].uv = { 1.0f, 0.0f };
	buffer[2].uv = { 0.0f, 1.0f };
	buffer[3].uv = { 1.0f, 1.0f };

	vertexBuffer->Unmap(0, nullptr);
	buffer = nullptr;

	matrix = DirectX::XMMatrixIdentity();
}

auto Square::update() -> void {
	using namespace DirectX;

	matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Fogo::Utility::Time::GetElapsedTime()));
}

auto Square::render(ID3D12GraphicsCommandList * commandList) const -> void {
	using namespace DirectX;

	//ルートシグネチャとPSOの設定
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(pipelineState.Get());

	//カメラの設定
	const XMMATRIX view = XMMatrixLookAtLH({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	const XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 640.0f / 480.0f, 1.0f, 20.0f);

	XMFLOAT4X4 out;
	XMStoreFloat4x4(&out, XMMatrixTranspose(matrix * view * projection));

	XMFLOAT4X4 * buffer { };

	if (FAILED(constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer)))) return;

	//行列を定数バッファに書き込み
	*buffer = out;

	constantBuffer->Unmap(0, nullptr);
	buffer = nullptr;

	const auto & vertexView = D3D12_VERTEX_BUFFER_VIEW{
		vertexBuffer->GetGPUVirtualAddress(),
		sizeof(TexturedVertex3D) * Option::vertexesCount,
		sizeof(TexturedVertex3D)
	};

	//定数バッファをシェーダのレジスタにセット
	commandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetGPUVirtualAddress());

	//テクスチャをシェーダのレジスタにセット
	ID3D12DescriptorHeap * heaps[] = { texture->getDescriptorHeap().Get() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	commandList->SetGraphicsRootDescriptorTable(1, texture->getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	//インデックスを使用しないトライアングルストリップで描画
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandList->IASetVertexBuffers(0, 1, &vertexView);

	//描画
	commandList->DrawInstanced(Option::vertexesCount, 1, 0, 0);
}
