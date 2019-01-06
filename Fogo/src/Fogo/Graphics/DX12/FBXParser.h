#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <fbxsdk.h>
#include "./Texture.h"

namespace Fogo::Graphics::DX12 {
	class FBXParser {
	public:
		struct Vertex {
			DirectX::XMFLOAT3 position;		// 座標
			DirectX::XMFLOAT3 normal;		// 法線
			DirectX::XMFLOAT2 texture;		// UV座標
			bool operator == (const Vertex & v) const {		//　＝＝演算子のオーバーロード 
				return std::memcmp(this, &v, sizeof(Vertex)) == 0;
			}
		};

		struct Material {
			std::string name;
			DirectX::XMFLOAT3 emissive, ambient, diffuse, specular;
			float transparency, shininess;
			std::shared_ptr<Texture> texture;
			std::string texturePath;
		};

		struct Mesh {
			std::vector<Vertex> vertexes;
			std::vector<unsigned int> indexes;
			Material material;
			const char * nodeName;
			const char * materialName;
			int skinCount;
		};

		static std::vector<int> GetIndexes(FbxMesh * mesh);
		static std::vector<DirectX::XMFLOAT3> GetPositions(FbxMesh* mesh, const std::vector<int> & indexes);
		static std::vector<DirectX::XMFLOAT3> GetNormals(FbxMesh* mesh, const std::vector<int> & indexes);
		static std::vector<DirectX::XMFLOAT2> GetUVs(FbxMesh* mesh, const std::vector<int> & indexes, int uvNo = 0);
		static Mesh Parse(FbxMesh * mesh);
		static Material Parse(FbxSurfaceMaterial * mesh);
		static std::vector<Material> GetMaterials(FbxScene * scene);
		static std::vector<Mesh> GetMeshes(FbxScene * scene);
	};
}


