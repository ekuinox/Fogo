#include "./FBXModel.h"
#include "../../../external/d3dx12.h"
#include "../../../Fogo.h"
#include <fbxsdk.h>
#include <iostream>

using namespace Fogo::Graphics::DX12;
using Microsoft::WRL::ComPtr;

struct MaterialProperty {
	FbxDouble3 result;
	std::vector<std::string> texturePaths;
};

static void ReadNode(FbxNode * parent, std::vector<FBXModel::Mesh> & meshes, const FBXModel::Properties & properties) noexcept;
static std::vector<FBXModel::Vertex> GetVertexes(FbxMesh * mesh, const FBXModel::Properties & properties) noexcept;
static FBXModel::Material GetMaterial(FbxSurfaceMaterial * material, const FBXModel::Properties & properties) noexcept;
static MaterialProperty GetMaterialProperty(const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName);

void ReadNode(FbxNode * parent, std::vector<FBXModel::Mesh> & meshes, const FBXModel::Properties & properties) noexcept {
	if (parent == nullptr) return;

	const auto children_count = parent->GetChildCount();
	for (auto i = 0; i < children_count; i++) {
		auto child = parent->GetChild(i);

		// マテリアル数1つにしか対応しない
		if (child->GetMaterialCount() == 1) {
			meshes.emplace_back(FBXModel::Mesh{
				GetVertexes(child->GetMesh(), properties),
				GetMaterial(child->GetMaterial(0), properties)
			});
		}

		// 更に子を処理
		ReadNode(child, meshes, properties);
	}
}

std::vector<FBXModel::Vertex> GetVertexes(FbxMesh * mesh, const FBXModel::Properties & properties) noexcept {
	if (mesh == nullptr) return {};

	std::vector<FBXModel::Vertex> vertexes;

	// 頂点座標と法線ベクトル獲得
	std::vector<FbxVector4> positions, normals;

	const auto polygons_count = mesh->GetPolygonCount(); // 面の数

	for (auto i = 0; i < polygons_count; i++) {
		
		const auto polygon_size = mesh->GetPolygonSize(i); // 頂点数
		for (auto j = 0; j < polygon_size; j++) {
			// 頂点座標
			positions.push_back(mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)));

			// 法線ベクトル
			FbxVector4 normal;
			if (mesh->GetPolygonVertexNormal(i, j, normal)) {
				normals.push_back(normal);
			}
		}
	}

	// 配列初期化
	vertexes.resize(positions.size());

	// 頂点座標と法線情報を格納
	for (size_t i = 0; i < vertexes.size(); ++i) {
		auto & position = positions[i];
		auto & normal = normals[i];

		vertexes[i].position = DirectX::XMFLOAT3(position.mData[0], position.mData[1], position.mData[2]);
		vertexes[i].normal = DirectX::XMFLOAT4(normal.mData[0], normal.mData[1], normal.mData[2], normal.mData[3]);
	}

	// UVセットの名前配列獲得
	FbxStringList uvset_names;
	mesh->GetUVSetNames(uvset_names);

	auto unmapped = false;
	auto uv_count = 0;

	const auto uvset_names_count = uvset_names.GetCount();
	for (auto i = 0; i < uvset_names_count; ++i) {
		for (auto j = 0; j < polygons_count; ++j) {
			const auto polygon_size = mesh->GetPolygonSize(j);
			for (auto k = 0; k < polygon_size; ++k) {
				FbxVector2 uv;
				mesh->GetPolygonVertexUV(j, k, uvset_names.GetStringAt(i), uv, unmapped);

				if (vertexes.size() > uv_count) {
					vertexes[uv_count].texture = DirectX::XMFLOAT2(uv.mData[0], uv.mData[1]);
				}
				uv_count++;
			}
		}
	}

	return vertexes;
}

FBXModel::Material GetMaterial(FbxSurfaceMaterial * material, const FBXModel::Properties & properties) noexcept {
	if (material == nullptr) return {};

	FBXModel::Material newMaterial;
	std::vector<std::string> texturePaths {};

	const auto & emissiveProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	const auto & ambientProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	const auto & diffuseProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	const auto & specularProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	
	newMaterial.emissive = DirectX::XMFLOAT3(emissiveProperty.result.mData[0], emissiveProperty.result.mData[1], emissiveProperty.result.mData[2]);
	newMaterial.ambient = DirectX::XMFLOAT3(ambientProperty.result.mData[0], ambientProperty.result.mData[1], ambientProperty.result.mData[2]);
	newMaterial.diffuse = DirectX::XMFLOAT3(diffuseProperty.result.mData[0], diffuseProperty.result.mData[1], diffuseProperty.result.mData[2]);
	newMaterial.specular = DirectX::XMFLOAT3(specularProperty.result.mData[0], specularProperty.result.mData[1], specularProperty.result.mData[2]);

	for (const auto & texturePath : emissiveProperty.texturePaths) texturePaths.emplace_back(texturePath);
	for (const auto & texturePath : ambientProperty.texturePaths) texturePaths.emplace_back(texturePath);
	for (const auto & texturePath : diffuseProperty.texturePaths) texturePaths.emplace_back(texturePath);
	for (const auto & texturePath : specularProperty.texturePaths) texturePaths.emplace_back(texturePath);

	const auto transparency_factor_property = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (transparency_factor_property.IsValid()) {
		newMaterial.transparency = static_cast<float>(transparency_factor_property.Get<FbxDouble>());
	}

	const auto shininess_property = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininess_property.IsValid()) {
		newMaterial.shininess = static_cast<float>(shininess_property.Get<FbxDouble>());
	}

	if (const auto implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX)) {
		const auto rootTable = implementation->GetRootTable();
		const auto entryCount = rootTable->GetEntryCount();

		for (unsigned int i = 0; i < entryCount; ++i) {
			const auto entry = rootTable->GetEntry(i);
			auto fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
			if (!fbxProperty.IsValid()) {
				fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
			}

			const auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
			if (textureCount > 0) {
				std::string src = entry.GetSource();

				for (unsigned int j = 0; j < textureCount; ++j) {
					if (src == "Maya|DiffuseTexture") {
						texturePaths.emplace_back(fbxProperty.GetSrcObject<FbxFileTexture>(j)->GetFileName());
					}
				}
			}
		}
	}

	if (!texturePaths.empty()) {
		const auto textureName = texturePaths.front().substr(texturePaths.front().find_last_of('/') + 1);
		const auto temp = properties.textureDirectory + std::wstring(textureName.begin(), textureName.end());
		const auto ext = temp.substr(temp.find_last_of('.') + 1);
		newMaterial.texture = std::make_shared<Texture>(
			temp.c_str(),
			ext == L"tga" || ext == L"TGA" ? Texture::Type::TGA : Texture::Type::ANY
		);
	}

	return newMaterial;
}

MaterialProperty GetMaterialProperty(const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName) {
	Fogo::Utility::Log(material->GetName());
	
	FbxDouble3 result(0, 0, 0);
	std::vector<std::string> texturePaths {};
	
	const auto factor_property = material->FindProperty(factorPropertyName);
	const auto property = material->FindProperty(propertyName);
	
	if (property.IsValid()) {

		if (factor_property.IsValid()) {
			result = property.Get<FbxDouble3>();
			if (const auto factor = factor_property.Get<FbxDouble>() != 1) {
				result[0] *= factor;
				result[1] *= factor;
				result[2] *= factor;
			}
		}

		const auto textures_count = property.GetSrcObjectCount<FbxFileTexture>();
		for (auto i = 0u; i < textures_count; i++) {
			if (const auto texture_file = property.GetSrcObject<FbxFileTexture>(i)) {
				std::string uv_name_string = texture_file->UVSet.Get().Buffer();
				texturePaths.emplace_back(texture_file->GetFileName());
			}
		}

		// layered textures
		const auto layered_textures_count = property.GetSrcObjectCount<FbxLayeredTexture>();
		for (auto i = 0u; i < layered_textures_count; i++) {
			const auto layered_texture = property.GetSrcObject<FbxLayeredTexture>(i);
			const auto texture_file_count = layered_texture->GetSrcObjectCount<FbxFileTexture>();
			for (auto j = 0u; j < texture_file_count; j++) {
				if (const auto texture_file = layered_texture->GetSrcObject<FbxFileTexture>(j)) {
					std::string uv_name_string = texture_file->UVSet.Get().Buffer();
					texturePaths.emplace_back(texture_file->GetFileName());
				}
			}
		}
	}

	return { result, texturePaths };
}

/* --- FBXModel Class Functions  --- */
void FBXModel::loadModel(const char * fileName)  {
	const auto manager = FbxManager::Create();
	const auto scene = FbxScene::Create(manager, "");
	const auto io = FbxIOSettings::Create(manager, IOSROOT);

	manager->SetIOSettings(io);

	const auto importer = FbxImporter::Create(manager, "");

	Utility::ExecOrFail(importer->Initialize(fileName, -1, manager->GetIOSettings()));
	Utility::ExecOrFail(importer->Import(scene));

	importer->Destroy();

	// シーンのものすべてを三角化
	{
		FbxGeometryConverter converter(manager);
		converter.Triangulate(scene, true);
	}
	
	ReadNode(scene->GetRootNode(), __meshes, __properties);

	io->Destroy();
	scene->Destroy();
	manager->Destroy();
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

	Fogo::Utility::ExecOrFail(D3D12SerializeRootSignature(
		&root_signature_desc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob,
		nullptr
	));

	Utility::ExecOrFail(Graphics::GetDevice()->CreateRootSignature(
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

	// パイプラインステートオブジェクト作成
	// 頂点シェーダとピクセルシェーダがないと、作成に失敗する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO;
	ZeroMemory(&descPSO, sizeof(descPSO));
	descPSO.InputLayout = { INPUT_LAYOUT, ARRAYSIZE(INPUT_LAYOUT) };										// インプットレイアウト設定
	descPSO.pRootSignature = __root_signature.Get();														// ルートシグニチャ設定
	descPSO.VS = { reinterpret_cast<BYTE*>(__properties.vertexShader->GetBufferPointer()), __properties.vertexShader->GetBufferSize() };	// 頂点シェーダ設定
	descPSO.PS = { reinterpret_cast<BYTE*>(__properties.pixelShader->GetBufferPointer()), __properties.pixelShader->GetBufferSize() };	// ピクセルシェーダ設定
	descPSO.RasterizerState = descRasterizer;																// ラスタライザ設定
	descPSO.BlendState = descBlend;																			// ブレンド設定
	descPSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);									// ステンシルバッファ有効設定
	descPSO.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;													// DSVフォーマット指定
	descPSO.SampleMask = UINT_MAX;																			// サンプルマスク設定
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;									// プリミティブタイプ	
	descPSO.NumRenderTargets = 1;																			// レンダーターゲット数
	descPSO.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;														// レンダーターゲットフォーマット
	descPSO.SampleDesc.Count = 1;																			// サンプルカウント
	Utility::ExecOrFail(Graphics::GetDevice()->CreateGraphicsPipelineState(&descPSO, IID_PPV_ARGS(__pipeline_state_object.GetAddressOf())));
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
	// ディスクリプタヒープとコマンドリストの関連づけ
	Graphics::GetCommandList()->SetDescriptorHeaps(DESCRIPTOR_HEAP_TYPE_SET, __descriptor_heaps[0].GetAddressOf());
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
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		
		descResource.Width = mesh.vertexes.size() * sizeof(Vertex);

		Graphics::GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&descResource,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource)
		);

		UINT8 * data_begin;
		Utility::SimplePromise([&] {
			return resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin));
		}).then([&] {
			const auto temp = reinterpret_cast<Vertex*>(data_begin);
			for (size_t i = 0; i < mesh.vertexes.size(); ++i)
			{
				temp[i] = mesh.vertexes[i];
			}
			resource->Unmap(0, nullptr);
		});

		__vertex_buffers.push_back({
			resource,
			D3D12_VERTEX_BUFFER_VIEW { // 頂点バッファビュー設定
				resource->GetGPUVirtualAddress(),
				static_cast<UINT>(descResource.Width),
				sizeof Vertex
			}
			});
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
	// コンスタントバッファビューを作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc_constant_buffer_view;

	desc_constant_buffer_view.BufferLocation = __constant_buffer_resource->GetGPUVirtualAddress();
	desc_constant_buffer_view.SizeInBytes = sizeof(MatrixConstantBuffer) + 255 & ~255;	// コンスタントバッファは256バイトアラインメントで配置する必要がある

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

FBXModel::FBXModel(const char * fileName, const Properties & properties)
	: __properties(properties), __meshes({}) {
	loadModel(fileName);
	compileShaders();
	createRootSignature();
	createPipelineStateObject();
	createDescriptorHeaps();
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

		// コンスタントバッファ更新
		using namespace DirectX;

		const auto view = XMMatrixLookAtLH({ 0, 0, -50 }, { 0, 0, 0 }, { 0, 1, 0 });
		const auto proj = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60),
			static_cast<float>(Utility::Window::GetWidth()) / static_cast<float>(Utility::Window::GetHeight()),
			1,
			1000
		);

		const MatrixConstantBuffer buffer{ XMMatrixTranspose(matrix * view * proj) };

		UINT8 * data_begin;
		Utility::SimplePromise([&] {
			return __constant_buffer_resource->Map(0, nullptr, reinterpret_cast<void**>(&data_begin));
		}).then([&] {
			memcpy(data_begin, &buffer, sizeof buffer);
			__constant_buffer_resource->Unmap(0, nullptr);
		});

		for (auto i = 0; i < __meshes.size(); ++i) {
			// コンスタントバッファを設定
			{
				std::vector<ID3D12DescriptorHeap*> heaps = { __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Get() };
				commandList->SetDescriptorHeaps(heaps.size(), heaps.data());
				commandList->SetGraphicsRootDescriptorTable(0, __descriptor_heaps[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetGPUDescriptorHandleForHeapStart());
			}
			// テクスチャをセット
			if (__meshes[i].material.texture) {
				std::vector<ID3D12DescriptorHeap*> heaps = { __meshes[i].material.texture->getDescriptorHeap().Get() };
				commandList->SetDescriptorHeaps(heaps.size(), heaps.data());
				commandList->SetGraphicsRootDescriptorTable(1, __meshes[i].material.texture->getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
			}
			// 三角形描画
			commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandList->IASetVertexBuffers(0, 1, &__vertex_buffers[i].view);
			commandList->DrawInstanced(__meshes[i].vertexes.size(), __meshes[i].vertexes.size() / 3, 0, 0);
		}
	});
}

