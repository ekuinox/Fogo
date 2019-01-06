#include "FBXParser.h"

using namespace Fogo::Graphics::DX12;
using namespace DirectX;

std::vector<int> FBXParser::GetIndexes(FbxMesh* mesh) {
	const auto count = mesh->GetPolygonCount();			// 三角形の数を取得
	std::vector<int> indexList;							// 頂点インデックス（面の構成情報）
	indexList.reserve(count * 3);				// 3角形の数×３

	// 面の構成情報を取得する
	for (unsigned int i = 0; i < count; i++) {
		indexList.push_back(mesh->GetPolygonVertex(i, 0));		// i番目の三角形の０番目の頂点インデックスを取得
		indexList.push_back(mesh->GetPolygonVertex(i, 1));		// i番目の三角形の１番目の頂点インデックスを取得
		indexList.push_back(mesh->GetPolygonVertex(i, 2));		// i番目の三角形の２番目の頂点インデックスを取得
	}
	return indexList;
}

std::vector<XMFLOAT3> FBXParser::GetPositions(FbxMesh* mesh, const std::vector<int> & indexes) {
	//　頂点座標を取得
	std::vector<XMFLOAT3> positionList {};					//	頂点座標格納ＶＥＣＴＯＲ 
	positionList.reserve(indexes.size());				//  

	for (unsigned int i = 0; i < indexes.size(); ++i) {
		const auto position = mesh->GetControlPointAt(indexes[i]); // index番目の頂点座標を取得
		if (position[3] != 0.0) {
			if (position[3] == 1.0) {
				positionList.emplace_back(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]));			// 同次座標化
			}
			else {
				positionList.emplace_back(static_cast<float>(position[0] / position[3]), static_cast<float>(position[1] / position[3]), static_cast<float>(position[2] / position[3]));		// 同次座標化
			}
		}
		else {
			positionList.emplace_back(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]));				// Ｗを無視して格納
		}
	}
	return positionList;
}

std::vector<XMFLOAT3> FBXParser::GetNormals(FbxMesh * mesh, const std::vector<int> & indexes) {
	const auto elementCount = mesh->GetElementNormalCount();			// 何個の法線情報がセットされているか

	assert(elementCount == 1);					// １個の法線情報にしか対応しない

	const auto element = mesh->GetElementNormal(0);	// ０番目の法線セットを取得
	const auto mappingmode = element->GetMappingMode();	// マッピングモード取得
	const auto referenceMode = element->GetReferenceMode();	// リファレンスモード取得
	const auto & indexArray = element->GetIndexArray();	// 法線情報を格納したｖｅｃｔｏｒ配列のインデックス値配列を取得 
	const auto & directArray = element->GetDirectArray();	// 法線情報を格納したｖｅｃｔｏｒ配列を取得

	// eDirectか　eIndexDirectのみ対応
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// 法線情報ＶＥＣＴＯＲ
	std::vector<XMFLOAT3> normalList;
	normalList.reserve(indexes.size());		// 頂点インデックス数分確保

	// 頂点座標でマッピングされている場合
	if (mappingmode == FbxGeometryElement::eByControlPoint) {
		for (const auto & index : indexes) {
			const auto normal = directArray.GetAt(
				referenceMode == FbxGeometryElement::eDirect ? index : indexArray.GetAt(index)
			); 		// 法線情報を取得
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.emplace_back(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]));			// 同次座標へ
				}
				else {
					normalList.emplace_back(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3]));		// 同次座標へ
				}
			}
		}
	}
	else if (mappingmode == FbxGeometryElement::eByPolygonVertex) {
		// 頂点インデックス(面の構成情報)でマッピングされている場合
		auto indexByPolygonVertex = 0;
		const auto polygonCount = mesh->GetPolygonCount();					// ポリゴン数を取得
		for (unsigned int i = 0; i < polygonCount; ++i) {
			const auto polygonSize = mesh->GetPolygonSize(i);				// 頂点数を取得
			for (unsigned int j = 0; j < polygonSize; ++j) {
				// リファレンスモードを判定
				auto normal = directArray.GetAt(
					referenceMode == FbxGeometryElement::eDirect ? indexByPolygonVertex : indexArray.GetAt(indexByPolygonVertex)
				); // 法線情報を取得
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0) {
						normalList.emplace_back(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]));		// 同次座標へ
					}
					else {
						normalList.emplace_back(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3]));	// 同次座標へ
					}
				}
				++indexByPolygonVertex;
			}
		}
	}
	else {
		assert(false);
	}

	return normalList;
}

std::vector<XMFLOAT2> FBXParser::GetUVs(FbxMesh * mesh, const std::vector<int> & indexes, int uvNo) {
	std::vector<XMFLOAT2> uvList {};

	const auto elementCount = mesh->GetElementUVCount();		// 何個のＵＶ情報がセットされているか
	if (uvNo + 1 > elementCount)						// 今回は１個にのみ対応
	{
		return uvList;
	}

	const auto element = mesh->GetElementUV(uvNo);					// UVセットを取得
	const auto mappingMode = element->GetMappingMode();		// マッピングモードを取得
	const auto referenceMode = element->GetReferenceMode();	// リファレンスモードを取得	
	const auto & indexArray = element->GetIndexArray();	// ＵＶ情報を格納したＶＥＣＴＯＲ配列のインデックス配列を取得
	const auto & directArray = element->GetDirectArray();	// ＵＶ値配列を取得

	// eDirctかeIndexDirectのみ対応
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// 頂点インデックス数分ｖｅｃｔｏｒ容量を確保
	uvList.reserve(indexes.size());

	// 頂点に対応して格納されている場合
	if (mappingMode == FbxGeometryElement::eByControlPoint)	{
		// 頂点座標でマッピング
		for (const auto & index : indexes) {
			// リファレンスモードを判定
			const auto uv = directArray.GetAt(
				referenceMode == FbxGeometryElement::eDirect ? index : indexArray.GetAt(index)
			);
			uvList.emplace_back(static_cast<float>(uv[0]), static_cast<float>(uv[1]));		// float値として格納
		}
	}
	// 面の構成情報に対応して格納されている場合
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
		// ポリゴンバーテックス（面の構成情報のインデックス）でマッピング
		auto indexByPolygonVertex = 0;						// 面の構成情報インデックス配列のインデックス
		const auto polygonCount = mesh->GetPolygonCount();			// メッシュのポリゴン数を取得
		for (unsigned int i = 0; i < polygonCount; ++i)				// ポリゴン数分ループ
		{
			const auto polygonSize = mesh->GetPolygonSize(i);		// ｉ番目のポリゴン頂点数を取得

			// ポリゴンの頂点数分ループ
			for (unsigned int j = 0; j < polygonSize; ++j)
			{
				// リファレンスモードの判定？
				const auto uv = directArray.GetAt(
					referenceMode == FbxGeometryElement::eDirect ? indexByPolygonVertex : indexArray.GetAt(indexByPolygonVertex)
				);
				uvList.emplace_back(static_cast<float>(uv[0]), static_cast<float>(uv[1]));	// ｆｌｏａｔ値として格納

				++indexByPolygonVertex;						// 頂点インデックスをインクリメント
			}
		}
	}
	else {
		// それ以外のマッピングモードには対応しない
		assert(false);
	}

	return uvList;
}


FBXParser::Mesh FBXParser::Parse(FbxMesh* mesh) {
	assert(mesh != nullptr);

	const auto node = mesh->GetNode();					// ＦＢＸノードを取得

	// マテリアルが1のものしか対応しない
	assert(node->GetMaterialCount() == 1);

	Mesh modelMesh;								// 最終的に保存されるメッシュ情報（この関数の出力値）

	// ワンスキンの数を取得
	modelMesh.skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);		// ワンスキン情報の数をセット
	modelMesh.nodeName = node->GetName();								// ＦＢＸノード名をセット
	modelMesh.materialName = node->GetMaterial(0)->GetName();			// マテリアル名をセット

	// インデックスＶＥＣＴＯＲ取得
	const auto indexes = GetIndexes(mesh);

	// 頂点データ取得
	const auto positions = GetPositions(mesh, indexes);		// 頂点座標を取得（面の構成情報に合わせる）
	const auto normals = GetNormals(mesh, indexes);			// 法線ベクトルを取得（面の構成情報に合わせる）
	const auto uvs = GetUVs(mesh, indexes);				// UV座標を取得（面の構成情報に合わせる）

	// 念のためサイズチェック

	assert(indexes.size() == positions.size());
	assert(indexes.size() == normals.size());
	assert(indexes.size() == uvs.size());

	// テンポラリの頂点情報（ＭｏｄｅｌＶｅｒｔｅｘ）を作る
	std::vector<Vertex> tempVertexes;
	tempVertexes.reserve(indexes.size());		// 頂点インデックスサイズ分容量を確保

	// 頂点インデックス数分（面の構成情報順）ループする
	for (unsigned int i = 0; i < indexes.size(); ++i) {
		tempVertexes.emplace_back(Vertex { positions[i], normals[i], uvs.empty() ? XMFLOAT2(0.0f, 0.0f) : uvs[i] });
	}

	// 重複頂点を除いてインデックス作成
	modelMesh.vertexes.reserve(tempVertexes.size());		// 頂点情報数分エリアを確保する

	modelMesh.indexes.reserve(indexes.size());		// 面の構成情報数分エリアを確保する

	// ここで重複している頂点データを除き、頂点データリストとインデックスリストを作り直している
	// 重複した頂点を含んだ頂点数分ループしている(面の構成情報を元に抜き出した頂点情報)
	for (const auto & vertex : tempVertexes) {
		// 重複しているか？
		// modelMesh.vertexListは、最初空でだんだん登録されていく（重複していない頂点情報として）
		auto it = std::find(modelMesh.vertexes.begin(), modelMesh.vertexes.end(), vertex);
		if (it == modelMesh.vertexes.end())	{
			// 重複していない
			modelMesh.indexes.push_back(modelMesh.vertexes.size());	// 頂点インデックスを格納
			modelMesh.vertexes.push_back(vertex);					// 頂点情報を格納
		}
		else {
			// 重複している
			modelMesh.indexes.push_back(std::distance(modelMesh.vertexes.begin(), it));// インデックス番号（重複頂点した先頭データを指し示している）をインデックスリストにセット
		}
	}

	return modelMesh;
}

FBXParser::Material FBXParser::Parse(FbxSurfaceMaterial* material) {
	auto modelMaterial = Material {};
	modelMaterial.name = material->GetName();

	// マテリアル情報取得
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
		const auto lambert = static_cast<FbxSurfaceLambert*>(material);
		modelMaterial.diffuse.x = static_cast<float>(lambert->Diffuse.Get()[0]);
		modelMaterial.diffuse.y = static_cast<float>(lambert->Diffuse.Get()[1]);
		modelMaterial.diffuse.z = static_cast<float>(lambert->Diffuse.Get()[2]);

		modelMaterial.ambient.x = static_cast<float>(lambert->Ambient.Get()[0]);
		modelMaterial.ambient.y = static_cast<float>(lambert->Ambient.Get()[1]);
		modelMaterial.ambient.z = static_cast<float>(lambert->Ambient.Get()[2]);

		modelMaterial.specular.x = 0.0f;
		modelMaterial.specular.y = 0.0f;
		modelMaterial.specular.z = 0.0f;
	}
	else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
		const auto phong = static_cast<FbxSurfacePhong*>(material);
		modelMaterial.diffuse.x = static_cast<float>(phong->Diffuse.Get()[0]);
		modelMaterial.diffuse.y = static_cast<float>(phong->Diffuse.Get()[1]);
		modelMaterial.diffuse.z = static_cast<float>(phong->Diffuse.Get()[2]);

		modelMaterial.ambient.x = phong->Ambient.Get()[0];
		modelMaterial.ambient.y = phong->Ambient.Get()[1];
		modelMaterial.ambient.z = phong->Ambient.Get()[2];

		modelMaterial.specular.x = phong->Specular.Get()[0];
		modelMaterial.specular.y = phong->Specular.Get()[1];
		modelMaterial.specular.z = phong->Specular.Get()[2];
	}
	else {
		modelMaterial.diffuse.x = 1.0f;
		modelMaterial.diffuse.y = 1.0f;
		modelMaterial.diffuse.z = 1.0f;

		modelMaterial.ambient.x = 1.0f;
		modelMaterial.ambient.y = 1.0f;
		modelMaterial.ambient.z = 1.0f;

		modelMaterial.specular.x = 0.0f;
		modelMaterial.specular.y = 0.0f;
		modelMaterial.specular.z = 0.0f;
	}

	// テクスチャを取得
	if (const auto implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX)) {
		const auto rootTable = implementation->GetRootTable();
		const auto entryCount = rootTable->GetEntryCount();

		for (unsigned int i = 0; i < entryCount; ++i) {
			const auto & entry = rootTable->GetEntry(i);
			auto fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
			if (!fbxProperty.IsValid()) {
				fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
			}

			const auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
			if (textureCount < 1) continue;

			std::string src = entry.GetSource();
			for (unsigned int j = 0; j < textureCount; ++j) {

				auto tex = fbxProperty.GetSrcObject<FbxFileTexture>(j);
				std::string texName = tex->GetFileName();
				texName = texName.substr(texName.find_last_of('/') + 1);

				if (src == "Maya|DiffuseTexture") {
					modelMaterial.texturePath = texName.substr(texName.find_last_of('/') + 1);
				}
			}
		}
	}

	return modelMaterial;
}

std::vector<FBXParser::Material> FBXParser::GetMaterials(FbxScene * scene) {
	std::vector<Material> materials {};
	const auto count = scene->GetMaterialCount();
	for (unsigned int i = 0; i < count; ++i) {
		materials.emplace_back(Parse(scene->GetMaterial(i)));
	}
	return materials;
}

std::vector<FBXParser::Mesh> FBXParser::GetMeshes(FbxScene * scene) {
	std::vector<Mesh> meshes {};
	const auto count = scene->GetMemberCount<FbxMesh>();
	for (unsigned int i = 0; i < count; ++i) {
		meshes.emplace_back(Parse(scene->GetMember<FbxMesh>(i)));
	}
	return meshes;
}
