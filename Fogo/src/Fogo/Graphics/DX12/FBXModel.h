#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "./Texture.h"

namespace Fogo::Graphics::DX12 {
	class FBXModel {
	public:
		struct Properties {
			Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
			LPCWSTR textureDirectory;
			Properties & setTextureDirectory(LPCWSTR newTextureDirectory) { textureDirectory = newTextureDirectory; return *this; }
			Properties & setVertexShader(const Microsoft::WRL::ComPtr<ID3DBlob> & newShader) { vertexShader = newShader; return *this; }
			Properties & setPixelShader(const Microsoft::WRL::ComPtr<ID3DBlob> & newShader) { pixelShader = newShader; return *this; }
		};

		struct Vertex {
			DirectX::XMFLOAT3 position;		// 座標
			DirectX::XMFLOAT4 normal;		// 法線
			DirectX::XMFLOAT2 texture;		// UV座標
		};

		struct Material {
			DirectX::XMFLOAT3 emissive, ambient, diffuse, specular;
			float transparency, shininess;
			std::shared_ptr<Texture> texture;
		};

		struct Mesh {
			std::vector<Vertex> vertexes;
			Material material;
		};

		struct MatrixConstantBuffer {
			DirectX::XMMATRIX matrix;
		};

		static constexpr UINT CONSTANT_BUFFER_NUMBER = 2;
		static constexpr D3D12_INPUT_ELEMENT_DESC INPUT_LAYOUT[3] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

	private:
		enum DESCRIPTOR_HEAP_TYPE
		{
			DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,	// UAVなど用
			DESCRIPTOR_HEAP_TYPE_SAMPLER,		// サンプラ用
			DESCRIPTOR_HEAP_TYPE_RTV,			// レンダーターゲット用	
			DESCRIPTOR_HEAP_TYPE_DSV,			// デプスステンシル用	
			DESCRIPTOR_HEAP_TYPE_MAX,			// ディスクリプタヒープ数
			DESCRIPTOR_HEAP_TYPE_SET = DESCRIPTOR_HEAP_TYPE_SAMPLER + 1,
		};

		Properties __properties;
		std::vector<Mesh> __meshes;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> __root_signature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> __pipeline_state_object;
		struct VertexBuffer {
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			D3D12_VERTEX_BUFFER_VIEW view;
		};
		std::vector<VertexBuffer> __vertex_buffers;
		Microsoft::WRL::ComPtr<ID3D12Resource> __constant_buffer_resource;
		D3D12_CPU_DESCRIPTOR_HANDLE __constant_buffer_handles[CONSTANT_BUFFER_NUMBER];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_MAX];
		std::shared_ptr<Fogo::Graphics::DX12::Texture> __texture;

		void loadModel(const char * fileName);
		void compileShaders();
		void createRootSignature();
		void createPipelineStateObject();
		void createDescriptorHeaps();
		void createVertexBuffers();
		void createConstantBuffer();

	public:
		DirectX::XMMATRIX matrix;

		FBXModel(const char * fileName, const Properties & properties = {});
		const std::vector<Mesh> & getMeshes() const;
		void render() const;
	};
}
