#include <cstdio>
#include <fbxsdk.h>
#include "FBX2016Loader.h"
#include <iostream>
#include "Fogo/Utility/Exception.h"

static void GetFBXVertexData(FbxMesh * pMesh, VertexContainer & outVertexData);
static void GetMaterialData(FbxSurfaceMaterial * material);
static void GetMeshData(FbxNode * child, VertexContainer & outVertexData);

void loadFBXConvertToVertexData(const char* filename, VertexContainer& outVertexData) {
	const auto manager = FbxManager::Create();
	const auto scene = FbxScene::Create(manager, "");
	const auto io = FbxIOSettings::Create(manager, IOSROOT);
	
	manager->SetIOSettings(io);
	
	const auto importer = FbxImporter::Create(manager, "");
	
	Fogo::Utility::ExecOrFail(importer->Initialize(filename, -1, manager->GetIOSettings()));
	Fogo::Utility::ExecOrFail(importer->Import(scene));
	
	importer->Destroy();

	// シーンのものすべてを三角化
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);

	GetMeshData(scene->GetRootNode(), outVertexData);
	
	io->Destroy();
	scene->Destroy();
	manager->Destroy();
}

// 再帰的にメッシュ情報を処理する
void GetMeshData(FbxNode * parent, VertexContainer & outVertexData)
{
	// メッシュだけ処理
	const auto children_count = parent->GetChildCount();
	for(auto i = 0; i < children_count; i++) {
		auto child = parent->GetChild(i);

		// メッシュを見つけたら
		if(child->GetMesh()) {
			// 頂点情報取得
			GetFBXVertexData(child->GetMesh(), outVertexData);
		}

		// マテリアル
		const auto material_count = child->GetMaterialCount();
		for(auto j = 0u; j < material_count; ++j) {
			const auto material = child->GetMaterial(j);
			if(material) {
				GetMaterialData(material);
			}
		}

		child->GetChild(0);

		// 更に子を処理
		GetMeshData(child, outVertexData);
	}
}

// 頂点データ獲得
void GetFBXVertexData(FbxMesh * mesh, VertexContainer & outVertexData)
{
	// 頂点座標と法線ベクトル獲得
	std::vector<FbxVector4> positions, normals;
	FbxVector4 normal;

	// メモ:GetPolygonCount = 面数、GetPolygonSize = 頂点数
	for(auto i = 0; i < mesh->GetPolygonCount(); i++) {
		for(auto j = 0; j < mesh->GetPolygonSize(i); j++) {
			// 頂点座標
			positions.push_back(mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)));

			// 法線ベクトル
			mesh->GetPolygonVertexNormal(i, j, normal);
			normals.push_back(normal);
		}
	}

	// 配列初期化
	outVertexData.resize(positions.size());

	// 頂点座標と法線情報を格納
	for(size_t i = 0; i < outVertexData.size(); ++i) {
		auto & pos		= positions[i];
		auto & normal	= normals[i];

		outVertexData[i].pos	= DirectX::XMFLOAT3(pos.mData[0], pos.mData[1] , pos.mData[2]);
		outVertexData[i].normal = DirectX::XMFLOAT4(normal.mData[0],  normal.mData[1],  normal.mData[2],  normal.mData[3]);
	}

	// UVセットの名前配列獲得
	FbxStringList uv_set_names;
	mesh->GetUVSetNames(uv_set_names);

	auto unmapped = false;
	auto uv_count = 0;

	for(auto i = 0; i < uv_set_names.GetCount(); ++i) {
		for(auto j = 0; j < mesh->GetPolygonCount(); ++j) {
			for(auto k = 0; k < mesh->GetPolygonSize(j); ++k) {
				FbxVector2 uv;
				mesh->GetPolygonVertexUV(j, k, uv_set_names.GetStringAt(i), uv, unmapped);

				if(outVertexData.size() > uv_count) {
					outVertexData[uv_count].uv = DirectX::XMFLOAT2(uv.mData[0], uv.mData[1]);
				}
				uv_count++;
			}
		}
	}
}

// マテリアルプロパティ獲得
FbxDouble3 GetMaterialProperty(
	const FbxSurfaceMaterial * material,
	const char * propertyName,
	const char * factorPropertyName)
{
	FbxDouble3 result(0, 0, 0);
	const auto property = material->FindProperty(propertyName);
	const auto factor_property = material->FindProperty(factorPropertyName);
	if(property.IsValid() && factor_property.IsValid()) {
		result = property.Get<FbxDouble3>();
		if(const auto factor = factor_property.Get<FbxDouble>() != 1) {
			result[0] *= factor;
			result[1] *= factor;
			result[2] *= factor;
		}
	}

	if(property.IsValid()) {
		const auto textures_count = property.GetSrcObjectCount<FbxFileTexture>();
		for(auto i = 0u; i < textures_count; i++) {
			if(const auto texture_file = property.GetSrcObject<FbxFileTexture>(i)) {
				std::string uv_name_string = texture_file->UVSet.Get().Buffer();
				std::string filepath = texture_file->GetFileName();
				std::cout << filepath.c_str() << std::endl;
			}
		}

		// layered textures
		const auto layered_textures_count = property.GetSrcObjectCount<FbxLayeredTexture>();
		for(auto i = 0u; i < layered_textures_count; i++) {
			const auto layered_texture = property.GetSrcObject<FbxLayeredTexture>(i);
			const auto texture_file_count = layered_texture->GetSrcObjectCount<FbxFileTexture>();
			for(auto j = 0u; j < texture_file_count; j++) {
				if(const auto texture_file = layered_texture->GetSrcObject<FbxFileTexture>(j)) {
					std::string uv_name_string = texture_file->UVSet.Get().Buffer();
					std::string filepath = texture_file->GetFileName();
					std::cout << filepath.c_str() << std::endl;
				}
			}
		}
	}

	return result;
}

// マテリアル表示
void GetMaterialData(FbxSurfaceMaterial* material) {
	if (material == nullptr) return;

	const auto emissive = GetMaterialProperty(material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	const auto ambient = GetMaterialProperty(material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	const auto diffuse = GetMaterialProperty(material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	const auto specular = GetMaterialProperty(material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	const auto transparency_factor_property = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if(transparency_factor_property.IsValid()) {
		auto transparencyFactor = transparency_factor_property.Get<FbxDouble>();
	}

	const auto shininess_property = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if(shininess_property.IsValid()) {
		auto shininess = shininess_property.Get<FbxDouble>();
	}
}
