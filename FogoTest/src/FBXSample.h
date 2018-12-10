#pragma once

#include "./FBX2016Loader.h"
#include "Fogo/Game/ComponentInterface.h"

class FBXSample : public Fogo::Game::ComponentInterface {
public:
	static constexpr UINT CONSTANT_BUFFER_NUMBER = 2;
	static constexpr D3D12_INPUT_ELEMENT_DESC INPUT_LAYOUT[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	struct MatrixConstantBuffer {
		DirectX::XMMATRIX matrix;
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

	Microsoft::WRL::ComPtr<ID3D12RootSignature> __root_signature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> __pipeline_state_object;
	Microsoft::WRL::ComPtr<ID3D12Resource> __vertex_buffer_resource;
	D3D12_VERTEX_BUFFER_VIEW __vertex_buffer_view;
	Microsoft::WRL::ComPtr<ID3D12Resource> __constant_buffer_resource;
	D3D12_VERTEX_BUFFER_VIEW __constant_buffer_view;
	D3D12_CPU_DESCRIPTOR_HANDLE __constant_buffer_handles[CONSTANT_BUFFER_NUMBER];
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_MAX];
	VertexContainer __vertexes;
	DirectX::XMMATRIX __matrix;

	void createRootSignature();
	void createPipelineStateObject(const Microsoft::WRL::ComPtr<ID3DBlob> & vertexShader, const Microsoft::WRL::ComPtr<ID3DBlob> & pixelShader);
	void createDescriptorHeaps();
	void createVertexBuffer();
	void createConstantBuffer();
	void initializeGameData();
public:
	FBXSample(const char * modelFileName, const Microsoft::WRL::ComPtr<ID3DBlob>& vertexShader, const Microsoft::WRL::ComPtr<ID3DBlob>
	          & pixelShader);
	void update() override;
	void render() const override;
};