#pragma once
#include <fbxsdk.h>
#include <memory>
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>
#include "../FBX.h"
#include "../Texture.h"

namespace Fogo::Graphics::DX12::DrawObject {
	class FBXModel {
	public:
		DirectX::XMMATRIX matrix {};
	private:
		struct Buffer {
			Microsoft::WRL::ComPtr<ID3D12Resource> vertex;
			Microsoft::WRL::ComPtr<ID3D12Resource> index;
			Microsoft::WRL::ComPtr<ID3D12Resource> constant;
		};

		std::vector<Buffer> __buffers;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> __pipeline_state;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> __root_signature;
		
		struct Vertex {
			DirectX::XMFLOAT3 position;						// XNA MATH���C�u�����Ŏg�p�ł���悤�ɒ�`���ꂽ�\���́@float*3 
			DirectX::XMFLOAT4 color;						// �J���[�l
			DirectX::XMFLOAT3 normal;					// �@���x�N�g��
			DirectX::XMFLOAT2 texture;						// �e�N�X�`�����W
			DirectX::XMFLOAT4 boneIndex;					// �{�[���s��ւ̃C���f�b�N�X�S��
			DirectX::XMFLOAT4 boneWeighs;				// �{�[���E�F�C�g�S��
		};

		// �萔�o�b�t�@��`
		struct ConstantBuffer {
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMFLOAT3 lightDirection;
			DirectX::XMFLOAT3 cameraPos;
		};

		struct ConstantBufferBoneMatrix { // �{�[���s��i�[�p�̃R���X�^���g�o�b�t�@
			DirectX::XMMATRIX boneMatrix[400];
		};

		struct ConstantBufferMaterial {
			DirectX::XMFLOAT4 diffuse;
			DirectX::XMFLOAT4 specular;
			DirectX::XMFLOAT4 ambient;
			DirectX::XMFLOAT4 power;
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> __constant_buffer, __bone_matrix_constant_buffer, __material_constant_buffer;

		struct Mesh {
			std::vector<Vertex> vertexes;
		};

		std::unique_ptr<FBX> __fbx;

		std::vector<FBX::Material> __materials;
		std::vector<Texture> __textures;

	public:
		FBXModel(
			const char * fileName,
			const Microsoft::WRL::ComPtr<ID3D12Device> & device,
			Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
			Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature
		);
		auto update() -> void;
		auto render(
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& projection
		) const -> void;
	};
}
