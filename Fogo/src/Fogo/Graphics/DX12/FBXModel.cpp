#include "./FBXModel.h"
#include "../../Utility/HelperFunctions.h"
#include <fbxsdk.h>
#include <iostream>

using namespace Fogo::Graphics::DX12;

struct MaterialProperty {
	FbxDouble3 result;
	std::vector<std::string> texturePathes;
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
	std::vector<std::string> texturePathes {};

	const auto & emissiveProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	const auto & ambientProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	const auto & diffuseProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	const auto & specularProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	
	newMaterial.emissive = DirectX::XMFLOAT3(emissiveProperty.result.mData[0], emissiveProperty.result.mData[1], emissiveProperty.result.mData[2]);
	newMaterial.ambient = DirectX::XMFLOAT3(ambientProperty.result.mData[0], ambientProperty.result.mData[1], ambientProperty.result.mData[2]);
	newMaterial.diffuse = DirectX::XMFLOAT3(diffuseProperty.result.mData[0], diffuseProperty.result.mData[1], diffuseProperty.result.mData[2]);
	newMaterial.specular = DirectX::XMFLOAT3(specularProperty.result.mData[0], specularProperty.result.mData[1], specularProperty.result.mData[2]);

	for (const auto & texturePath : emissiveProperty.texturePathes) texturePathes.emplace_back(texturePath);
	for (const auto & texturePath : ambientProperty.texturePathes) texturePathes.emplace_back(texturePath);
	for (const auto & texturePath : diffuseProperty.texturePathes) texturePathes.emplace_back(texturePath);
	for (const auto & texturePath : specularProperty.texturePathes) texturePathes.emplace_back(texturePath);

	const auto transparency_factor_property = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (transparency_factor_property.IsValid()) {
		newMaterial.transparency = static_cast<float>(transparency_factor_property.Get<FbxDouble>());
	}

	const auto shininess_property = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininess_property.IsValid()) {
		newMaterial.shininess = static_cast<float>(shininess_property.Get<FbxDouble>());
	}

	std::wstring stemp = properties.textureDirectory + std::wstring(texturePathes.front().begin(), texturePathes.front().end());

	newMaterial.texture = std::make_shared<Texture>(stemp.c_str());
	//if (texturePathes.size() > 0) newMaterial.texturePath = texturePathes.front();

	return newMaterial;
}

MaterialProperty GetMaterialProperty(const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName) {
	std::cout << propertyName << ", " << factorPropertyName << std::endl;
	FbxDouble3 result(0, 0, 0);
	const auto property = material->FindProperty(propertyName);
	const auto factor_property = material->FindProperty(factorPropertyName);
	if (property.IsValid() && factor_property.IsValid()) {
		result = property.Get<FbxDouble3>();
		if (const auto factor = factor_property.Get<FbxDouble>() != 1) {
			result[0] *= factor;
			result[1] *= factor;
			result[2] *= factor;
		}
	}

	std::vector<std::string> texturePathes {};

	if (property.IsValid()) {
		const auto textures_count = property.GetSrcObjectCount<FbxFileTexture>();
		for (auto i = 0u; i < textures_count; i++) {
			if (const auto texture_file = property.GetSrcObject<FbxFileTexture>(i)) {
				std::string uv_name_string = texture_file->UVSet.Get().Buffer();
				texturePathes.emplace_back(texture_file->GetFileName());
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
					texturePathes.emplace_back(texture_file->GetFileName());
				}
			}
		}
	}

	return { result, texturePathes };
}

/* --- FBXModel Class Functions  --- */
void FBXModel::loadModel(const char * fileName) noexcept  {
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



FBXModel::FBXModel(const char * fileName, const Properties & properties)
	: __properties(properties), __meshes({}) {
	loadModel(fileName);
}

const std::vector<FBXModel::Mesh>& FBXModel::getMeshes() const
{
	return __meshes;
}

