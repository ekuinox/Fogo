#include "Car.h"

auto Car::createRootSignature() -> void {
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
	rootParameters[0].Descriptor = D3D12_ROOT_DESCRIPTOR { 0, 0 };

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable = D3D12_ROOT_DESCRIPTOR_TABLE { 1, &range[0] };

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

	const auto & rootSignatureDesc = D3D12_ROOT_SIGNATURE_DESC{
		_countof(rootParameters),
		rootParameters,
		1,
		&SAMPLER_DESC,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};

	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	Fogo::Utility::ExecOrFail(D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob,
		nullptr
	));

	Fogo::Utility::ExecOrFail(Fogo::Graphics::DX12::Graphics::GetDevice()->CreateRootSignature(
		0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	));
}
auto Car::createPipelineStateObject() -> void {
	const auto & vertexShader = Fogo::Graphics::DX12::Graphics::CompileVertexShader(L"resources/unitychan.hlsl", 0, "VSMain", "vs_5_0");
	const auto & pixelShader = Fogo::Graphics::DX12::Graphics::CompilePixelShader(L"resources/unitychan.hlsl", 0, "PSMain", "ps_5_0");

	/*
	constexpr D3D12_INPUT_ELEMENT_DESC inputElementDesc[3]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	*/
	constexpr D3D12_INPUT_ELEMENT_DESC inputElementDesc[6] {
		// �Z�}���e�B�b�N���@index�@�@ format�@�@�@�@�@�@�@�@�@�@�@   slot�@�@byteoffset�@�@�@�@�@     
		{ "POSITION",        0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      0,										D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// ���_���W
		{ "COLOR",           0,      DXGI_FORMAT_R32G32B32A32_FLOAT,   0,      4 * 3,									D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// ���_�J���[
		{ "NORMAL",          0,      DXGI_FORMAT_R32G32B32_FLOAT,      0,      4 * 4 + 4 * 3 ,							D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// �@���x�N�g��
		{ "TEXCOORD",        0,      DXGI_FORMAT_R32G32_FLOAT,         0,      4 * 3 + 4 * 4 + 4 * 3,					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// �e�N�X�`�����W
		{ "BONEINDEX",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 2 + 4 * 3 + 4 * 4 + 4 * 3	,		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// �{�[���s��C���f�N�b�X
		{ "BONEWEIGHT",		 0,		 DXGI_FORMAT_R32G32B32A32_FLOAT,   0,	   4 * 4 + 4 * 2 + 4 * 3 + 4 * 4 + 4 * 3,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	// �{�[���E�F�C�g
	};
	

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc { };
	
	pipelineStateDesc.pRootSignature = rootSignature.Get();

	pipelineStateDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
	pipelineStateDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };

	pipelineStateDesc.BlendState.AlphaToCoverageEnable = FALSE;
	pipelineStateDesc.BlendState.IndependentBlendEnable = FALSE;
	for (auto& render_target : pipelineStateDesc.BlendState.RenderTarget) {
		render_target = D3D12_RENDER_TARGET_BLEND_DESC {
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
		};
	}

	pipelineStateDesc.SampleMask = UINT_MAX;

	pipelineStateDesc.RasterizerState = D3D12_RASTERIZER_DESC{
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
	};

	pipelineStateDesc.DepthStencilState = D3D12_DEPTH_STENCIL_DESC {
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
	};

	pipelineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC { inputElementDesc,	_countof(inputElementDesc) };

	pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineStateDesc.NumRenderTargets = 1;

	//pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	pipelineStateDesc.SampleDesc = DXGI_SAMPLE_DESC { 1, 0 };

	pipelineStateDesc.NodeMask = 0;

	pipelineStateDesc.CachedPSO = {};

	pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	Fogo::Utility::ExecOrFail(Fogo::Graphics::DX12::Graphics::GetDevice()->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pipelineState)));
}

Car::Car(const char * fileName) {
	createRootSignature();
	createPipelineStateObject();

	__model = std::make_unique<Fogo::Graphics::DX12::DrawObject::FBXModel>(fileName, Fogo::Graphics::DX12::Graphics::GetDevice(), pipelineState, rootSignature);
}

auto Car::update() -> void {
	
}

auto Car::render() const -> void
{
	Fogo::Graphics::DX12::Graphics::Render([&](Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) {
		__model->render(
			commandList,
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }),
			DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 640.0f / 480.0f, 1.0f, 20.0f)
		);
	});
}