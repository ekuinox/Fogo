#include "FBXSample.h"
#include "Fogo.h"
#include "d3dx12.h"

using namespace Fogo;
using namespace Graphics;
using namespace DirectX;
using namespace Microsoft::WRL;

void FBXSample::createRootSignature() {
	// ���_�V�F�[�_�ɃR���X�^���g�o�b�t�@��n���郋�[�g�V�O�j�`���쐬
	D3D12_DESCRIPTOR_RANGE range;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range.NumDescriptors = 1;
	range.BaseShaderRegister = 0;
	range.RegisterSpace = 0;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_DESCRIPTOR_TABLE root_descriptor_table;
	root_descriptor_table.NumDescriptorRanges = 1;
	root_descriptor_table.pDescriptorRanges = &range;

	D3D12_ROOT_PARAMETER root_parameter;
	root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	root_parameter.DescriptorTable = root_descriptor_table;

	
	ComPtr<ID3DBlob> out_blob;
	auto desc_root_signature = D3D12_ROOT_SIGNATURE_DESC();
	desc_root_signature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	desc_root_signature.NumParameters = 1;
	desc_root_signature.pParameters = &root_parameter;
	D3D12SerializeRootSignature(&desc_root_signature, D3D_ROOT_SIGNATURE_VERSION_1, &out_blob, nullptr);
	DX12::Graphics::GetDevice()->CreateRootSignature(
		0x00000001,
		out_blob->GetBufferPointer(),
		out_blob->GetBufferSize(),
		IID_PPV_ARGS(__root_signature.GetAddressOf())
	);
}

void FBXSample::createPipelineStateObject(const ComPtr<ID3DBlob>& vertexShader, const ComPtr<ID3DBlob>& pixelShader) {
	// ���C�X�^���C�U�[�X�e�[�g�ݒ�
	// �f�t�H���g�̐ݒ�͈ȉ��̃y�[�W���Q��(ScissorEnable�͂Ȃ����ǁE�E�E)
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d3d12%2FD3D12_RASTERIZER_DESC);k(D3D12_RASTERIZER_DESC);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	D3D12_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	descRasterizer.CullMode = D3D12_CULL_MODE_NONE;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 0;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthBiasClamp = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	descRasterizer.ForcedSampleCount = 0;
	descRasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// �u�����h�X�e�[�g�ݒ�
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d3d12%2FD3D12_BLEND_DESC);k(D3D12_BLEND_DESC);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	D3D12_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].LogicOpEnable = FALSE;
	descBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�쐬
	// ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���Ȃ��ƁA�쐬�Ɏ��s����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO;
	ZeroMemory(&descPSO, sizeof(descPSO));
	descPSO.InputLayout = { INPUT_LAYOUT, ARRAYSIZE(INPUT_LAYOUT) };										// �C���v�b�g���C�A�E�g�ݒ�
	descPSO.pRootSignature = __root_signature.Get();														// ���[�g�V�O�j�`���ݒ�
	descPSO.VS = { reinterpret_cast<BYTE*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };	// ���_�V�F�[�_�ݒ�
	descPSO.PS = { reinterpret_cast<BYTE*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };	// �s�N�Z���V�F�[�_�ݒ�
	descPSO.RasterizerState = descRasterizer;																// ���X�^���C�U�ݒ�
	descPSO.BlendState = descBlend;																			// �u�����h�ݒ�
	descPSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);									// �X�e���V���o�b�t�@�L���ݒ�
	descPSO.SampleMask = UINT_MAX;																			// �T���v���}�X�N�ݒ�
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;									// �v���~�e�B�u�^�C�v	
	descPSO.NumRenderTargets = 1;																			// �����_�[�^�[�Q�b�g��
	descPSO.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;														// �����_�[�^�[�Q�b�g�t�H�[�}�b�g
	descPSO.SampleDesc.Count = 1;																			// �T���v���J�E���g
	DX12::Graphics::GetDevice()->CreateGraphicsPipelineState(&descPSO, IID_PPV_ARGS(__pipeline_state_object.GetAddressOf()));
}

void FBXSample::createDescriptorHeaps() {
	// descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.NumDescriptors = 2; // back buffer count

	for (auto i = 0u; i < DESCRIPTOR_HEAP_TYPE_MAX; ++i)
	{
		heapDesc.Flags = (i == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || i == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i);
		DX12::Graphics::GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(__descriptor_heaps[i].GetAddressOf()));
	}

	// �f�B�X�N���v�^�q�[�v�ƃR�}���h���X�g�̊֘A�Â�
	DX12::Graphics::GetCommandList()->SetDescriptorHeaps(DESCRIPTOR_HEAP_TYPE_SET, __descriptor_heaps[0].GetAddressOf());
}

void FBXSample::createVertexBuffer() {
	D3D12_HEAP_PROPERTIES heap_properties;
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.CreationNodeMask = 1;
	heap_properties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc_resource;
	desc_resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc_resource.Alignment = 0;
	desc_resource.Width = __vertexes.size() * sizeof(VertexData);
	desc_resource.Height = 1;
	desc_resource.DepthOrArraySize = 1;
	desc_resource.MipLevels = 1;
	desc_resource.Format = DXGI_FORMAT_UNKNOWN;
	desc_resource.SampleDesc.Count = 1;
	desc_resource.SampleDesc.Quality = 0;
	desc_resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc_resource.Flags = D3D12_RESOURCE_FLAG_NONE;

	DX12::Graphics::GetDevice()->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&desc_resource,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(__vertex_buffer_resource.GetAddressOf()));

	UINT8 * data_begin;
	if (SUCCEEDED(__vertex_buffer_resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin))))
	{
		const auto temp = reinterpret_cast<VertexData*>(data_begin);
		for (size_t i = 0; i < __vertexes.size(); ++i)
		{
			temp[i] = __vertexes[i];
		}
		__vertex_buffer_resource->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�ݒ�
	__vertex_buffer_view.BufferLocation = __vertex_buffer_resource->GetGPUVirtualAddress();
	__vertex_buffer_view.StrideInBytes = sizeof(VertexData);
	__vertex_buffer_view.SizeInBytes = desc_resource.Width;
}

void FBXSample::createConstantBuffer() {
	D3D12_HEAP_PROPERTIES heap_properties;
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.CreationNodeMask = 1;
	heap_properties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc_resource;
	desc_resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc_resource.Alignment = 0;
	desc_resource.Width = sizeof(MatrixConstantBuffer);
	desc_resource.Height = 1;
	desc_resource.DepthOrArraySize = 1;
	desc_resource.MipLevels = 1;
	desc_resource.Format = DXGI_FORMAT_UNKNOWN;
	desc_resource.SampleDesc.Count = 1;
	desc_resource.SampleDesc.Quality = 0;
	desc_resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc_resource.Flags = D3D12_RESOURCE_FLAG_NONE;

	DX12::Graphics::GetDevice()->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&desc_resource,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(__constant_buffer_resource.GetAddressOf()));
	// �R���X�^���g�o�b�t�@�r���[���쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc_constant_buffer_view;

	desc_constant_buffer_view.BufferLocation = __constant_buffer_resource->GetGPUVirtualAddress();
	desc_constant_buffer_view.SizeInBytes = sizeof(MatrixConstantBuffer) + 255 & ~255;	// �R���X�^���g�o�b�t�@��256�o�C�g�A���C�������g�Ŕz�u����K�v������

	DX12::Graphics::GetDevice()->CreateConstantBufferView(
		&desc_constant_buffer_view,
		__descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart()
	);

	const auto stride_handle_bytes = DX12::Graphics::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto i = 0u; i < CONSTANT_BUFFER_NUMBER; ++i) {
		__constant_buffer_handles[i] = __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart();
		__constant_buffer_handles[i].ptr += i * stride_handle_bytes;

		DX12::Graphics::GetDevice()->CreateConstantBufferView(&desc_constant_buffer_view, __constant_buffer_handles[i]);
	}
}

void FBXSample::initializeGameData() {
	__matrix = XMMatrixIdentity();
}

FBXSample::FBXSample(const char * modelFileName, const ComPtr<ID3DBlob> & vertexShader, const ComPtr<ID3DBlob> & pixelShader) {
	createRootSignature();
	createPipelineStateObject(vertexShader, pixelShader);
	createDescriptorHeaps();
	LoadFBXConvertToVertexData(modelFileName, __vertexes);
	createVertexBuffer();
	createConstantBuffer();
	initializeGameData();
}

void FBXSample::update() {
	__matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Utility::Time::GetElapsedTime()));
}

void FBXSample::render() const {
	DX12::Graphics::Render([&](ComPtr<ID3D12GraphicsCommandList> commandList) {
		commandList->SetGraphicsRootSignature(__root_signature.Get());
		commandList->SetPipelineState(__pipeline_state_object.Get());

		// �R���X�^���g�o�b�t�@�X�V
		using namespace DirectX;

		const auto view = XMMatrixLookAtLH({ 0, 0, -50 }, { 0, 0, 0 }, { 0, 1, 0 });
		const auto proj = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60),
			static_cast<float>(Utility::Window::GetWidth()) / static_cast<float>(Utility::Window::GetHeight()),
			1,
			1000
		);
		{
			const MatrixConstantBuffer buffer { XMMatrixTranspose(__matrix * view * proj) };

			UINT8 * data_begin;
			Utility::Window::SimplePromise([&] {
				return __constant_buffer_resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin));
			}).then([&] {
				memcpy(data_begin, &buffer, sizeof buffer);
				__constant_buffer_resource->Unmap(0, nullptr);
			});
		
			// �R���X�^���g�o�b�t�@��ݒ�
			ID3D12DescriptorHeap* heaps[] = { __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Get() };
			commandList->SetDescriptorHeaps(std::size(heaps), heaps);
			commandList->SetGraphicsRootDescriptorTable(0, __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetGPUDescriptorHandleForHeapStart());

			// �O�p�`�`��
			commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandList->IASetVertexBuffers(0, 1, &__vertex_buffer_view);
			commandList->DrawInstanced(__vertexes.size(), __vertexes.size() / 3, 0, 0);
		}
	});
	

}
