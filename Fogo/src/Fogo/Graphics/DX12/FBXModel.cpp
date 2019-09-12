#include "./FBXModel.h"
#include "../../../external/d3dx12.h"
#include "../../../Fogo.h"
#include "../../Game/ContainerBase.h"
#include "D3D12ExtendedStructs.h"
#include "DXGIExtendedStructs.h"
#include <fbxsdk.h>
#include <iostream>

using namespace Fogo;
using Microsoft::WRL::ComPtr;

using MeshesMemoContainer = ContainerBase<const char *, std::vector<FBXParser::Mesh>>;

void FBXModel::loadModel(const char * fileName)  {
	if (MeshesMemoContainer::shared.count(fileName) == 0) {
		const auto & meshes = FBXParser().import(fileName).triangulate().parse().loadTextures(__properties.textureDirectory).getMeshes();
		MeshesMemoContainer::shared[fileName] = meshes;
	}

	for (const auto & mesh : MeshesMemoContainer::shared[fileName]) {
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
	static constexpr D3D12_DESCRIPTOR_RANGE RANGES[2] = {
		D3D12DescriptorRangeExtended()
			.withRangeType(D3D12_DESCRIPTOR_RANGE_TYPE_CBV)
			.withNumDescriptors(1)
			.withOffsetInDescriptorsFromTableStart(D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND),
		D3D12DescriptorRangeExtended()
			.withRangeType(D3D12_DESCRIPTOR_RANGE_TYPE_SRV)
			.withNumDescriptors(1)
			.withOffsetInDescriptorsFromTableStart(D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	};

	static constexpr D3D12_STATIC_SAMPLER_DESC SAMPLER_DESC = D3D12StaticSamplerDescExtended()
		.withFilter(D3D12_FILTER_MIN_MAG_MIP_LINEAR)
		.withAddressU(D3D12_TEXTURE_ADDRESS_MODE_WRAP)
		.withAddressV(D3D12_TEXTURE_ADDRESS_MODE_WRAP)
		.withAddressW(D3D12_TEXTURE_ADDRESS_MODE_WRAP)
		.withMipLODBias(0.0f)
		.withMaxAnisotropy(16)
		.withComparisonFunc(D3D12_COMPARISON_FUNC_NEVER)
		.withBorderColor(D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK)
		.withMinLOD(0.0f)
		.withMaxLOD(D3D12_FLOAT32_MAX)
		.withShaderRegister(0)
		.withRegisterSpace(0)
		.withShaderVisibility(D3D12_SHADER_VISIBILITY_ALL);

	const D3D12_ROOT_PARAMETER root_parameters[2] = {
		D3D12RootParameterExtended()
			.withParameterType(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			.withShaderVisibility(D3D12_SHADER_VISIBILITY_VERTEX)
			.withDescriptorTable(D3D12RootDescriptorTableExtended().withNumDescriptorRanges(1).withDescriptorRanges(&RANGES[0])),
		D3D12RootParameterExtended()
			.withParameterType(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			.withShaderVisibility(D3D12_SHADER_VISIBILITY_ALL)
			.withDescriptorTable(D3D12RootDescriptorTableExtended().withNumDescriptorRanges(1).withDescriptorRanges(&RANGES[1])),
	};

	const D3D12_ROOT_SIGNATURE_DESC root_signature_desc = D3D12RootSignatureDescExtended()
		.withNumParameters(_countof(root_parameters))
		.withParameters(root_parameters)
		.withNumStaticSamplers(1)
		.withStaticSamplers(&SAMPLER_DESC)
		.withFlags(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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
	static constexpr D3D12_RASTERIZER_DESC RASTERIZER_DESC = D3D12RasterizerDescExtended()
		.withFillMode(D3D12_FILL_MODE_SOLID)
		.withCullMode(D3D12_CULL_MODE_NONE)
		.withFrontCounterClockwise(FALSE)
		.withDepthBias(0)
		.withDepthBiasClamp(0.0f)
		.withDepthClipEnable(TRUE)
		.withMultisampleEnable(FALSE)
		.withAntialiasedLineEnable(FALSE)
		.withForcedSampleCount(0)
		.withConservativeRaster(D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
	
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d3d12%2FD3D12_BLEND_DESC);k(D3D12_BLEND_DESC);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	static constexpr auto BLEND_DESC = D3D12BlendDescExtended()
		.withAlphaToCoverageEnable(FALSE)
		.withIndependentBlendEnable(FALSE)
		.withRenderTarget0(D3D12RenderTargetBlendDescExtended()
			.withBlendEnable(TRUE)
			.withLogicOpEnable(FALSE)
			.withSrcBlend(D3D12_BLEND_SRC_ALPHA)
			.withDestBlend(D3D12_BLEND_INV_SRC_ALPHA)
			.withBlendOp(D3D12_BLEND_OP_ADD)
			.withSrcBlendAlpha(D3D12_BLEND_ONE)
			.withDestBlendAlpha(D3D12_BLEND_ZERO)
			.withBlendOpAlpha(D3D12_BLEND_OP_ADD)
			.withLogicOp(D3D12_LOGIC_OP_AND)
			.withRenderTargetWriteMask(D3D12_COLOR_WRITE_ENABLE_ALL));

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�쐬
	// ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���Ȃ��ƁA�쐬�Ɏ��s����
	const D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateObjectDesc = D3D12GraphicsPipelineStateDescExtended()
		.withInputLayout( // �C���v�b�g���C�A�E�g�ݒ�
			D3D12InputLayoutDescExtended()
			.withInputElementDescs(FBXParser::Vertex::INPUT_LAYOUT)
			.withNumElements(ARRAYSIZE(FBXParser::Vertex::INPUT_LAYOUT)))
		.withRasterizerState(RASTERIZER_DESC) // ���X�^���C�U�ݒ�
		.withBlendState(BLEND_DESC) // �u�����h�ݒ�
		.withDSVFormat(DXGI_FORMAT_D32_FLOAT) // DSV�t�H�[�}�b�g�w��
		.withSampleMask(UINT_MAX) // �T���v���}�X�N�ݒ�
		.withPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE) // �v���~�e�B�u�^�C�v	
		.withNumRenderTargets(1) // �����_�[�^�[�Q�b�g��
		.withRTVFormat0(DXGI_FORMAT_B8G8R8A8_UNORM) // �����_�[�^�[�Q�b�g�t�H�[�}�b�g
		.withSampleDesc(DXGISampleDescExtended().withCount(1).withQuality(0)) // �T���v���J�E���g
		.withDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT)) // �X�e���V���o�b�t�@�L���ݒ�
		.withRootSignature(__root_signature.Get()) // ���[�g�V�O�j�`���ݒ�
		.withVS(D3D12ShaderBytecodeExtended::FromBlob(__properties.vertexShader.Get())) // ���_�V�F�[�_�ݒ�
		.withPS(D3D12ShaderBytecodeExtended::FromBlob(__properties.pixelShader.Get())); // �s�N�Z���V�F�[�_�ݒ�
	
	ExecOrFail(Graphics::GetDevice()->CreateGraphicsPipelineState(&pipelineStateObjectDesc, IID_PPV_ARGS(__pipeline_state_object.GetAddressOf())));
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

void FBXModel::FlushMemos() {
	MeshesMemoContainer::shared.clear();
}
