#include "FBXModel.h"
#include <vector>
#include <iostream>
#include <array>
#include <DirectXTex.h>
#include "../../../Utility/Time.h"
#include "../../../Utility/Exception.h"

#pragma comment(lib, "libfbxsdk-md.lib")

using namespace Fogo::Graphics::DX12::DrawObject;

FBXModel::FBXModel(
	const char * fileName,
	const Microsoft::WRL::ComPtr<ID3D12Device> & device,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature
) : __pipeline_state(std::move(pipelineState)), __root_signature(std::move(rootSignature)) {
	__fbx = std::make_unique<FBX>(fileName);

	const auto & meshContainer = __fbx->getModelMeshContainer();

	static constexpr D3D12_HEAP_PROPERTIES HEAP_PROPERTIES {
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	static constexpr D3D12_RESOURCE_DESC RESOURCE_DESC{
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		256,
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR
	};

	// ���_�o�b�t�@�̍쐬
	for (const auto & mesh : meshContainer) {
		std::vector<Vertex> vertexes;
		vertexes.resize(mesh.vertexList.size());
		for (auto i = 0; i < vertexes.size(); ++i) {
			// ���_���W���Z�b�g
			vertexes[i].position.x = mesh.vertexList[i].position.x;
			vertexes[i].position.y = mesh.vertexList[i].position.y;
			vertexes[i].position.z = mesh.vertexList[i].position.z;

			// ���_�J���[���Z�b�g
			vertexes[i].color.w = 1.0f;
			vertexes[i].color.x = 1.0f;
			vertexes[i].color.y = 1.0f;
			vertexes[i].color.z = 1.0f;

			// �@���x�N�g�����Z�b�g
			vertexes[i].normal.x = mesh.vertexList[i].normal.x;
			vertexes[i].normal.y = mesh.vertexList[i].normal.y;
			vertexes[i].normal.z = mesh.vertexList[i].normal.z;

			// �e�N�X�`�����W���Z�b�g
			vertexes[i].texture.x = mesh.vertexList[i].uv0.x;
			vertexes[i].texture.y = 1.0f - mesh.vertexList[i].uv0.y;

			// �{�[���s��C���f�b�N�X���Z�b�g
			vertexes[i].boneIndex.w = mesh.vertexList[i].boneIndex[0];
			vertexes[i].boneIndex.x = mesh.vertexList[i].boneIndex[1];
			vertexes[i].boneIndex.y = mesh.vertexList[i].boneIndex[2];
			vertexes[i].boneIndex.z = mesh.vertexList[i].boneIndex[3];

			// �{�[���E�F�C�g�l���Z�b�g
			vertexes[i].boneWeighs.w = mesh.vertexList[i].boneWeight.v[0];
			vertexes[i].boneWeighs.x = mesh.vertexList[i].boneWeight.v[1];
			vertexes[i].boneWeighs.y = mesh.vertexList[i].boneWeight.v[2];
			vertexes[i].boneWeighs.z = mesh.vertexList[i].boneWeight.v[3];
		}

		std::vector<WORD> indexList;
		indexList.resize(mesh.indexList.size());
		for (auto i = 0; i < indexList.size(); ++i) indexList[i] = mesh.indexList[i];

		__buffers.emplace_back();

		Utility::ExecOrFail(device->CreateCommittedResource(
			&HEAP_PROPERTIES,
			D3D12_HEAP_FLAG_NONE,
			&RESOURCE_DESC,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(__buffers.back().vertex.GetAddressOf())
		));

		Utility::ExecOrFail(device->CreateCommittedResource(
			&HEAP_PROPERTIES,
			D3D12_HEAP_FLAG_NONE,
			&RESOURCE_DESC,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(__buffers.back().constant.GetAddressOf())
		));

		Utility::ExecOrFail(device->CreateCommittedResource(
			&HEAP_PROPERTIES,
			D3D12_HEAP_FLAG_NONE,
			&RESOURCE_DESC,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(__buffers.back().index.GetAddressOf())
		));

		Utility::ExecOrFail(__buffers.back().vertex->Map(0, nullptr, reinterpret_cast<void**>(vertexes.data())));
		__buffers.back().vertex->Unmap(0, nullptr);
		Utility::ExecOrFail(__buffers.back().index->Map(0, nullptr, reinterpret_cast<void**>(indexList.data())));
		__buffers.back().index->Unmap(0, nullptr);
	}

	Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(__bone_matrix_constant_buffer.GetAddressOf())
	));

	Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(__constant_buffer.GetAddressOf())
	));

	Utility::ExecOrFail(device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(__material_constant_buffer.GetAddressOf())
	));


	// �V�F�[�_�[���\�[�X�r���[����
	for (const auto & mesh : meshContainer) {
		__materials.emplace_back(__fbx->getMaterial(mesh.materialName));
		
		printf("texture filename %s", __materials.back().diffuseTextureName.c_str());

		// �e�N�X�`���ǂݍ���
		const auto filePath = std::string("resources/Textures/" + __materials.back().diffuseTextureName);
		const auto ms = filePath.c_str();
		wchar_t ws[512];
		size_t ret;

		setlocale(LC_CTYPE, "jpn");
		mbstowcs_s(&ret, ws, 512, ms, _TRUNCATE);

		__textures.emplace_back(Texture(ws, Texture::Type::TGA));
	}
}

auto FBXModel::update() -> void {
	using namespace DirectX;

	matrix *= XMMatrixRotationY(XMConvertToRadians(360 * Utility::Time::GetElapsedTime()));
}

auto FBXModel::render(
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
	const DirectX::XMMATRIX & view,
	const DirectX::XMMATRIX & projection
) const -> void {
	using namespace DirectX;

	{
		ConstantBuffer * buffer { };

		__constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));

		//�s���萔�o�b�t�@�ɏ�������
		buffer->world = XMMatrixTranspose(matrix);
		buffer->view = XMMatrixTranspose(view); // �]�u���Đݒ�i�J�����ϊ��s��j
		buffer->projection = XMMatrixTranspose(projection); // �]�u���Đݒ�i�v���W�F�N�V�����ϊ��s��j
		buffer->lightDirection = XMFLOAT3(1, 1, 1); // ���̕����x�N�g���Z�b�g	
		buffer->cameraPos.x = 0.0f; // �J�����̈ʒu
		buffer->cameraPos.y = 0.0f;
		buffer->cameraPos.z = -1500.0f;

		__constant_buffer->Unmap(0, nullptr);
		
		commandList->SetGraphicsRootConstantBufferView(0, __constant_buffer->GetGPUVirtualAddress());
	}

	const UINT frame = 0;

	std::array<FBX::Matrix4x4, 400> boneMatrixList { };

	const auto & meshes = __fbx->getModelMeshContainer();
	
	for (auto i = 0u; i < meshes.size(); ++i) {
		const auto & mesh = meshes[i];
		// �����X�L���̃��b�V���łȂ��ꍇ
		if (mesh.skincount == 0) {
			{
				__fbx->getMeshMatrix(frame, i, boneMatrixList[0]);

				ConstantBufferBoneMatrix cb { };
				for (auto k = 0; k < 1; k++) {
					cb.boneMatrix[k] = XMMATRIX(
						boneMatrixList[k].m[0], boneMatrixList[k].m[1], boneMatrixList[k].m[2], boneMatrixList[k].m[3],
						boneMatrixList[k].m[4], boneMatrixList[k].m[5], boneMatrixList[k].m[6], boneMatrixList[k].m[7],
						boneMatrixList[k].m[8], boneMatrixList[k].m[9], boneMatrixList[k].m[10], boneMatrixList[k].m[11],
						boneMatrixList[k].m[12], boneMatrixList[k].m[13], boneMatrixList[k].m[14], boneMatrixList[k].m[15]);

					cb.boneMatrix[k] = XMMatrixTranspose(cb.boneMatrix[k]); // �]�u���Ȃ��Ƃ�������
				}

				auto buffer = new ConstantBufferBoneMatrix;

				__bone_matrix_constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));

				//�s���萔�o�b�t�@�ɏ�������
				for (auto j = 0u; j < 1; ++j) {
					buffer->boneMatrix[j] = cb.boneMatrix[j];
				}

				__bone_matrix_constant_buffer->Unmap(0, nullptr);
			}

			{
				// �}�e���A����������
				const auto & material = __fbx->getMaterial(mesh.materialName);

				// �R���X�^���g�o�b�t�@���̃}�e���A�����X�V
				ConstantBufferMaterial cb { };
				cb.diffuse.x = material.Diffuse.v[0];
				cb.diffuse.y = material.Diffuse.v[1];
				cb.diffuse.z = material.Diffuse.v[2];
				cb.diffuse.w = material.Diffuse.v[3];

				cb.specular.x = material.Speqular.v[0];
				cb.specular.y = material.Speqular.v[1];
				cb.specular.z = material.Speqular.v[2];
				cb.specular.w = material.Speqular.v[3];

				cb.ambient.x = material.Ambient.v[0];
				cb.ambient.y = material.Ambient.v[1];
				cb.ambient.z = material.Ambient.v[2];
				cb.ambient.w = material.Ambient.v[3];

				cb.power.x = material.Power;
				cb.power.y = material.Power;
				cb.power.z = material.Power;
				cb.power.w = material.Power;

				ConstantBufferMaterial * buffer;

				__bone_matrix_constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));

				//�s���萔�o�b�t�@�ɏ�������
				*buffer = cb;

				__bone_matrix_constant_buffer->Unmap(0, nullptr);
			}			
		}
		// �����X�L���̃��b�V���̏ꍇ
		else {
			{
				__fbx->getBoneMatrix(frame, i, boneMatrixList.data(), boneMatrixList.size());
				ConstantBufferBoneMatrix cb { };
				for (auto k = 0; k < 400; k++) {
					cb.boneMatrix[k] = XMMATRIX(
						boneMatrixList[k].m[0], boneMatrixList[k].m[1], boneMatrixList[k].m[2], boneMatrixList[k].m[3],
						boneMatrixList[k].m[4], boneMatrixList[k].m[5], boneMatrixList[k].m[6], boneMatrixList[k].m[7],
						boneMatrixList[k].m[8], boneMatrixList[k].m[9], boneMatrixList[k].m[10], boneMatrixList[k].m[11],
						boneMatrixList[k].m[12], boneMatrixList[k].m[13], boneMatrixList[k].m[14], boneMatrixList[k].m[15]
					);

					cb.boneMatrix[k] = XMMatrixTranspose(cb.boneMatrix[k]); // �]�u���Ȃ��Ƃ�������
				}
				ConstantBufferBoneMatrix * buffer;

				__bone_matrix_constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));

				//�s���萔�o�b�t�@�ɏ�������
				*buffer = cb;

				__bone_matrix_constant_buffer->Unmap(0, nullptr);
			}

			{
				// ���̃}�e���A���������}�e���A����������
				const auto & material = __fbx->getMaterial(mesh.materialName);

				//
				// �R���X�^���g�o�b�t�@���̃}�e���A�����X�V
				//
				ConstantBufferMaterial cb { };
				cb.diffuse.x = material.Diffuse.v[0];
				cb.diffuse.y = material.Diffuse.v[1];
				cb.diffuse.z = material.Diffuse.v[2];
				cb.diffuse.w = material.Diffuse.v[3];

				cb.specular.x = material.Speqular.v[0];
				cb.specular.y = material.Speqular.v[1];
				cb.specular.z = material.Speqular.v[2];
				cb.specular.w = material.Speqular.v[3];

				cb.ambient.x = material.Ambient.v[0];
				cb.ambient.y = material.Ambient.v[1];
				cb.ambient.z = material.Ambient.v[2];
				cb.ambient.w = material.Ambient.v[3];

				cb.power.x = material.Power;
				cb.power.y = material.Power;
				cb.power.z = material.Power;
				cb.power.w = material.Power;

				ConstantBufferMaterial * buffer;

				__bone_matrix_constant_buffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));

				//�s���萔�o�b�t�@�ɏ�������
				*buffer = cb;

				__bone_matrix_constant_buffer->Unmap(0, nullptr);
			}
		}

		//�萔�o�b�t�@���V�F�[�_�̃��W�X�^�ɃZ�b�g
		commandList->SetGraphicsRootConstantBufferView(0, __bone_matrix_constant_buffer->GetGPUVirtualAddress());
		commandList->SetGraphicsRootConstantBufferView(0, __material_constant_buffer->GetGPUVirtualAddress());

		//�e�N�X�`�����V�F�[�_�̃��W�X�^�ɃZ�b�g
		{
			std::vector<ID3D12DescriptorHeap*> heaps;
			for (const auto & texture : __textures)
				heaps.emplace_back(texture.getDescriptorHeap().Get());
			commandList->SetDescriptorHeaps(heaps.size(), heaps.data());
			for (const auto & texture : __textures)
				commandList->SetGraphicsRootDescriptorTable(1, texture.getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
		}

		//�`��
		commandList->DrawIndexedInstanced(mesh.indexList.size(), 1, 0, 0, 0);
	}
}