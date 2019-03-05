#include "./FBXModel.h"
#include "../../../external/d3dx12.h"
#include "../../../Fogo.h"
#include <fbxsdk.h>
#include <iostream>

using namespace Fogo;
using Microsoft::WRL::ComPtr;

void FBXModel::loadModel(const char * fileName)  {
	for (const auto & mesh : FBXParser()
		.import(fileName)
		.triangulate()
		.parse()
		.loadTextures(__properties.textureDirectory)
		.getMeshes()) {
		__meshes.emplace_back(mesh);
	}
}

void FBXModel::compileShaders() {
	if (__properties.vertexShader.Get() == nullptr) {

	}

	if (__properties.pixelShader.Get() == nullptr) {

	}
}

void FBXModel::createRootSignature() {
	static constexpr D3D12_DESCRIPTOR_RANGE range[2] = {
		D3D12_DESCRIPTOR_RANGE {
			D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
			1,
			0,
			0,
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		},
		D3D12_DESCRIPTOR_RANGE {
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			1,
			0,
			0,
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		}
	};

	D3D12_ROOT_PARAMETER root_parameters[2];

	root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	root_parameters[0].DescriptorTable = D3D12_ROOT_DESCRIPTOR_TABLE{ 1, &range[0] };

	root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	root_parameters[1].DescriptorTable = D3D12_ROOT_DESCRIPTOR_TABLE{ 1, &range[1] };

	static constexpr D3D12_STATIC_SAMPLER_DESC SAMPLER_DESC{
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

	const D3D12_ROOT_SIGNATURE_DESC root_signature_desc{
		_countof(root_parameters),
		root_parameters,
		1,
		&SAMPLER_DESC,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};

	ComPtr<ID3DBlob> blob;

	ExecOrFail(D3D12SerializeRootSignature(
		&root_signature_desc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob,
		nullptr
	));

	ExecOrFail(Graphics::GetDevice()->CreateRootSignature(
		0x00000001,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(__root_signature.GetAddressOf())
	));
}

void FBXModel::createPipelineStateObject() {
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

	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d3d12%2FD3D12_BLEND_DESC);k(D3D12_BLEND_DESC);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	D3D12_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].LogicOpEnable = FALSE;
	descBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_AND;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�쐬
	// ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���Ȃ��ƁA�쐬�Ɏ��s����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO;
	ZeroMemory(&descPSO, sizeof(descPSO));
	descPSO.InputLayout = { FBXParser::Vertex::INPUT_LAYOUT, ARRAYSIZE(FBXParser::Vertex::INPUT_LAYOUT) };										// �C���v�b�g���C�A�E�g�ݒ�
	descPSO.pRootSignature = __root_signature.Get();														// ���[�g�V�O�j�`���ݒ�
	descPSO.VS = { reinterpret_cast<BYTE*>(__properties.vertexShader->GetBufferPointer()), __properties.vertexShader->GetBufferSize() };	// ���_�V�F�[�_�ݒ�
	descPSO.PS = { reinterpret_cast<BYTE*>(__properties.pixelShader->GetBufferPointer()), __properties.pixelShader->GetBufferSize() };	// �s�N�Z���V�F�[�_�ݒ�
	descPSO.RasterizerState = descRasterizer;																// ���X�^���C�U�ݒ�
	descPSO.BlendState = descBlend;																			// �u�����h�ݒ�
	descPSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);									// �X�e���V���o�b�t�@�L���ݒ�
	descPSO.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;													// DSV�t�H�[�}�b�g�w��
	descPSO.SampleMask = UINT_MAX;																			// �T���v���}�X�N�ݒ�
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;									// �v���~�e�B�u�^�C�v	
	descPSO.NumRenderTargets = 1;																			// �����_�[�^�[�Q�b�g��
	descPSO.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;														// �����_�[�^�[�Q�b�g�t�H�[�}�b�g
	descPSO.SampleDesc.Count = 1;																			// �T���v���J�E���g
	ExecOrFail(Graphics::GetDevice()->CreateGraphicsPipelineState(&descPSO, IID_PPV_ARGS(__pipeline_state_object.GetAddressOf())));
}

void FBXModel::createDescriptorHeaps() {
	// descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.NumDescriptors = 2; // back buffer count

	for (auto i = 0u; i < DESCRIPTOR_HEAP_TYPE_MAX; ++i) {
		heapDesc.Flags = (i == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || i == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i);
		Graphics::GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(__descriptor_heaps[i].GetAddressOf()));
	}
	// �f�B�X�N���v�^�q�[�v�ƃR�}���h���X�g�̊֘A�Â�
	Graphics::GetCommandList()->SetDescriptorHeaps(DESCRIPTOR_HEAP_TYPE_SET, __descriptor_heaps[0].GetAddressOf());
}

void FBXModel::createIndexBuffers() {
	static constexpr D3D12_HEAP_PROPERTIES heapProperties {
		D3D12_HEAP_TYPE_UPLOAD, // Type
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN, // CPUPageProperty
		D3D12_MEMORY_POOL_UNKNOWN, // MemoryPoolPreference
		1, // CreationNodeMask
		1 // VisibleNodeMask
	};

	D3D12_RESOURCE_DESC descResource {
		D3D12_RESOURCE_DIMENSION_BUFFER, //Dimension
		0, // Alignment
		256, // Width
		1, // Height
		1, // DepthOrArraySize
		1, // MipLevels
		DXGI_FORMAT_UNKNOWN, // Format
		DXGI_SAMPLE_DESC { 1, 0 }, // SampleDesc { Count, Quality }
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, // Layout
		D3D12_RESOURCE_FLAG_NONE // Flags
	};

	for (auto && mesh : __meshes) {
		ComPtr<ID3D12Resource> resource;

		descResource.Width = mesh.indexes.size() * sizeof(unsigned int);

		Graphics::GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&descResource,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource)
		);

		UINT8 * data_begin;
		if (SUCCEEDED(resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin)))) {
			const auto temp = reinterpret_cast<unsigned int*>(data_begin);
			for (size_t i = 0; i < mesh.indexes.size(); ++i) {
				temp[i] = mesh.indexes[i];
			}
			resource->Unmap(0, nullptr);
		}

		mesh.indexBuffer = {
			resource,
			D3D12_INDEX_BUFFER_VIEW { // �C���f�b�N�X�o�b�t�@�r���[�ݒ�
				resource->GetGPUVirtualAddress(),
				static_cast<UINT>(descResource.Width),
				DXGI_FORMAT_R32_UINT
			}
		};
	}
}

void FBXModel::createVertexBuffers() {
	static constexpr D3D12_HEAP_PROPERTIES heapProperties {
		D3D12_HEAP_TYPE_UPLOAD, // Type
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN, // CPUPageProperty
		D3D12_MEMORY_POOL_UNKNOWN, // MemoryPoolPreference
		1, // CreationNodeMask
		1 // VisibleNodeMask
	};

	D3D12_RESOURCE_DESC descResource {
		D3D12_RESOURCE_DIMENSION_BUFFER, //Dimension
		0, // Alignment
		256, // Width
		1, // Height
		1, // DepthOrArraySize
		1, // MipLevels
		DXGI_FORMAT_UNKNOWN, // Format
		DXGI_SAMPLE_DESC { 1, 0 }, // SampleDesc { Count, Quality }
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, // Layout
		D3D12_RESOURCE_FLAG_NONE // Flags
	};

	for (auto && mesh : __meshes) {
		ComPtr<ID3D12Resource> resource;
		
		descResource.Width = mesh.vertexes.size() * sizeof(FBXParser::Vertex);

		Graphics::GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&descResource,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource)
		);

		UINT8 * data_begin;
		if (SUCCEEDED(resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin)))) {
			const auto temp = reinterpret_cast<FBXParser::Vertex*>(data_begin);
			for (size_t i = 0; i < mesh.vertexes.size(); ++i) {
				temp[i] = mesh.vertexes[i];
				temp[i].texture.y = 1.0f - temp[i].texture.y;
			}
			resource->Unmap(0, nullptr);
		}

		mesh.vertexBuffer = {
			resource,
			D3D12_VERTEX_BUFFER_VIEW { // ���_�o�b�t�@�r���[�ݒ�
				resource->GetGPUVirtualAddress(),
				static_cast<UINT>(descResource.Width),
				sizeof FBXParser::Vertex
			}
		};
	}
}

void FBXModel::createConstantBuffer() {
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

	Graphics::GetDevice()->CreateCommittedResource(
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

	Graphics::GetDevice()->CreateConstantBufferView(
		&desc_constant_buffer_view,
		__descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart()
	);

	const auto stride_handle_bytes = Graphics::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto i = 0u; i < CONSTANT_BUFFER_NUMBER; ++i) {
		__constant_buffer_handles[i] = __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart();
		__constant_buffer_handles[i].ptr += i * stride_handle_bytes;
		Graphics::GetDevice()->CreateConstantBufferView(&desc_constant_buffer_view, __constant_buffer_handles[i]);
	}
}

FBXModel::FBXModel(const char * fileName, const Properties & properties)
	: __properties(properties), __meshes({}) {
	loadModel(fileName);
	compileShaders();
	createRootSignature();
	createPipelineStateObject();
	createDescriptorHeaps();
	createIndexBuffers();
	createVertexBuffers();
	createConstantBuffer();
}

const std::vector<FBXModel::Mesh>& FBXModel::getMeshes() const
{
	return __meshes;
}

void FBXModel::render() const {
	Graphics::Render([&](ComPtr<ID3D12GraphicsCommandList> commandList) {
		commandList->SetGraphicsRootSignature(__root_signature.Get());
		commandList->SetPipelineState(__pipeline_state_object.Get());

		const MatrixConstantBuffer buffer { XMMatrixTranspose(world * view * projection) };

		UINT8 * data_begin;
		if (SUCCEEDED(__constant_buffer_resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin)))) {
			memcpy(data_begin, &buffer, sizeof buffer);
			__constant_buffer_resource->Unmap(0, nullptr);
		}

		for (const auto & mesh : __meshes) {
			// �R���X�^���g�o�b�t�@��ݒ�
			{
				std::vector<ID3D12DescriptorHeap*> heaps = { __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Get() };
				commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
				commandList->SetGraphicsRootDescriptorTable(0, __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetGPUDescriptorHandleForHeapStart());
			}
			// �e�N�X�`�����Z�b�g
			if (mesh.material.texture) {
				std::vector<ID3D12DescriptorHeap*> heaps = {mesh.material.texture->getDescriptorHeap().Get() };
				commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
				commandList->SetGraphicsRootDescriptorTable(1, mesh.material.texture->getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
			}
			// �O�p�`�`��
			commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandList->IASetVertexBuffers(0, 1, &mesh.vertexBuffer.view);
			commandList->IASetIndexBuffer(&mesh.indexBuffer.view);
			commandList->DrawIndexedInstanced(static_cast<UINT>(mesh.indexes.size()), 1, 0, 0, 0);
		}
	});
}
