#pragma once

#include <DirectXMath.h>
#include <vector>
#include <unordered_map>
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

	private:
		static std::vector<int> GetIndexes(FbxMesh * mesh);
		static std::vector<DirectX::XMFLOAT3> GetPositions(FbxMesh* mesh, const std::vector<int> & indexes);
		static std::vector<DirectX::XMFLOAT3> GetNormals(FbxMesh* mesh, const std::vector<int> & indexes);
		static std::vector<DirectX::XMFLOAT2> GetUVs(FbxMesh* mesh, const std::vector<int> & indexes, int uvNo = 0);
		static Mesh Parse(FbxMesh * mesh);
		static Material Parse(FbxSurfaceMaterial * mesh);
		static std::vector<Mesh> Parse(FbxScene * scene);
		static std::unordered_map<std::string, Material> GetMaterials(FbxScene * scene);
		static std::vector<Mesh> GetMeshes(FbxScene * scene);
		static FbxScene * Triangulate(FbxScene * scene, FbxManager * manager, bool isLegacy = false);

		std::vector<Mesh> __meshes;
		FbxScene * __scene;
		FbxManager * __manager;
		FbxIOSettings * __io_settings;

	public:
		FBXParser();
		FBXParser(const FBXParser &) = delete;
		FBXParser & operator=(const FBXParser &) = delete;
		FBXParser & import(const char * file);
		FBXParser & parse();
		FBXParser & triangulate(bool isLegacy = false);
		FBXParser & loadTextures(LPCWSTR directory);
		[[nodiscard]] std::vector<Mesh> getMeshes();
		~FBXParser();
	};
}


