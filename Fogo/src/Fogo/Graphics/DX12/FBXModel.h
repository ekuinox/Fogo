#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "./FBXParser.h"

namespace Fogo {
	class FBXModel {
	public:
		struct Properties {
			Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
			LPCWSTR textureDirectory;
			Properties & setTextureDirectory(LPCWSTR newTextureDirectory) { textureDirectory = newTextureDirectory; return *this; }
			Properties & setVertexShader(const Microsoft::WRL::ComPtr<ID3DBlob> & newShader) { vertexShader = newShader; return *this; }
			Properties & setPixelShader(const Microsoft::WRL::ComPtr<ID3DBlob> & newShader) { pixelShader = newShader; return *this; }
		};

		struct MatrixConstantBuffer {
			DirectX::XMMATRIX matrix;
		};

		static constexpr UINT CONSTANT_BUFFER_NUMBER = 2;

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

		template <typename View> struct Buffer {
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			View view;
		};

		struct Mesh : FBXParser::Mesh {
			Mesh(const FBXParser::Mesh & mesh) : FBXParser::Mesh(mesh) {}
			Buffer< D3D12_INDEX_BUFFER_VIEW> indexBuffer;
			Buffer< D3D12_VERTEX_BUFFER_VIEW> vertexBuffer;
		};

		Properties __properties;
		std::vector<Mesh> __meshes;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> __root_signature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> __pipeline_state_object;
		Microsoft::WRL::ComPtr<ID3D12Resource> __constant_buffer_resource;
		D3D12_CPU_DESCRIPTOR_HANDLE __constant_buffer_handles[CONSTANT_BUFFER_NUMBER];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_MAX];
		std::shared_ptr<Texture> __texture;

		void loadModel(const char * fileName);
		void compileShaders();
		void createRootSignature();
		void createPipelineStateObject();
		void createDescriptorHeaps();
		void createIndexBuffers();
		void createVertexBuffers();
		void createConstantBuffer();

	public:
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX view;

		FBXModel(const char * fileName, const Properties & properties = {});
		const std::vector<Mesh> & getMeshes() const;
		void render() const;
	};
}
