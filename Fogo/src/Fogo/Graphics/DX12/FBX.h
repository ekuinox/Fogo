#pragma once

#pragma once
#include <windows.h>
#include <fbxsdk.h> 
#include <string>
#include <vector>
#include <map>

namespace Fogo::Graphics::DX12 {
	class FBX {
	public:
		struct Matrix4x4 {
			union {
				float m[16];				// １次元配列
				float m2[4][4];			// ２次元配列
			};
			void transpose() {
				m2[0][1] = m2[1][0];
				m2[0][2] = m2[2][0];
				m2[0][3] = m2[3][0];
				m2[1][2] = m2[2][1];
				m2[1][3] = m2[3][1];
				m2[2][3] = m2[3][2];
			}
		};

		struct Vector3 {
			float x, y, z;
			Vector3(float p1, float p2, float p3) {
				x = p1; y = p2; z = p3;
			}
			Vector3() {
				x = y = z = 0.0f;
			}
		};

		struct Vector2 {
			float	x, y;
			Vector2(float p1, float p2) {
				x = p1; y = p2;
			}
			Vector2() {
				x = y = 0.0f;
			}
		};

		struct Vector4 {
			float v[4];
			Vector4(float x, float y, float z, float w) {
				v[0] = x;
				v[1] = y;
				v[2] = z;
				v[3] = w;
			}
			Vector4() {
				v[0] = v[1] = v[2] = v[3] = 0.0f;
			}
		};

		// ボーン情報
		struct BoneWeight {
			unsigned int boneIndex[4];				// ボーンインデックス番号
			Vector4 boneWeight;					// ボーンウエイト値
		};

		// 頂点情報構造体
		struct Vertex {
			Vector3 position;						// 座標
			Vector3 normal;							// 法線情報
			Vector2 uv0;							// テクスチャ座標
			unsigned int	boneIndex[4];			// 使用するボーン行列のインデックス
			Vector4			boneWeight;				// ウェイト値
			bool operator == (const Vertex & v) const {		//　＝＝演算子のオーバーロード 
				return std::memcmp(this, &v, sizeof(Vertex)) == 0;
			}
			Vertex() : boneIndex{} {
				boneWeight = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				position = Vector3(0.0f, 0.0f, 0.0f);
				normal = Vector3(0.0f, 0.0f, 0.0f);
				uv0 = Vector2(0.0f, 0.0f);
			}
		};

		// メッシュ情報構造体	
		struct Mesh {
			int							skincount;			// ワンスキンの数
			std::string					nodeName;			// FBXノード名
			std::string					materialName;		// 使用マテリアル名

			std::vector<Vertex>	vertexList;			// 頂点情報格納ＶＥＣＴＯＲコンテナ
			std::vector<unsigned int>	indexList;			// 面の構成情報ＶＥＣＴＯＲコンテナ
			Matrix4x4					invMeshBaseposeMatrix;	// メッシュの初期姿勢
			std::vector<std::string>	boneNodeNameList;		// ボーンノード名格納ＶＥＣＴＯＲコンテナ
			std::vector<Matrix4x4> 		invBoneBaseposeMatrixList;	// ボーンの初期姿勢の逆行列格納ＶＥＣＴＯＲコンテナ
		};

		struct Material {
			std::string materialName;
			std::string diffuseTextureName;
			std::string normalTextureName;
			std::string specularTextureName;
			std::string falloffTextureName;
			std::string reflectionMapTextureName;

			Vector4		Diffuse;		//ディフューズ
			Vector4		Speqular;		//スペキュラ
			Vector4		Ambient;		//アンビエント
			float		Power;			//反射の強さ
		};

		FBX(const char* modelFile, const char* animationFile = nullptr);
		~FBX();
		void getBoneMatrix(INT64 frame, int meshId, Matrix4x4* out_matrixList, unsigned int matrixCount);
		void getMeshMatrix(INT64 frame, int meshId, Matrix4x4& out_matrix);
		void loadAnimation(const char* animationfilename);
		std::vector<Mesh> & getModelMeshContainer();
		Material& getMaterial(std::string materialname);
		bool isAnimationEnd(INT64 frame);

	private:
		Matrix4x4 mutable matrix4x4Identity = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		};

		std::vector<Mesh>	meshList;					// ModelMeshを格納したｖｅｃｔｏｒ
		std::map<std::string, int> nodeIdDictionary;		// メッシュ名からmeshList番号を取得するためのＭＡＰ
		std::vector<Material> materialList;			// ModelMaterialを格納したｖｅｃｔｏｒ
		std::map<std::string, int> materialIdDictionary;	// マテリアル名からmaterialList番号を取得するためのＭＡＰ

		// FBXパース用
		FbxManager*		manager;		// SDKのメモリ管理
		FbxScene*		scene;			// ノードやメッシュ、マテリアル情報を含んだシーン情報
		FbxImporter*	importer;		// FBXを解析してFbxSceneに展開

		// アニメーション用
		FbxScene* fbxSceneAnimation;	// アニメーション情報を含んだシーン情報
		std::map<std::string, int> nodeIdDictionaryAnimation;		// ボーン名からノード番号を取得するためのＭＡＰ
		INT64 animationStartFrame = 0;		// 開始フレーム番号
		INT64 animationEndFrame = 0;		// 終了フレーム番号
	};
}
