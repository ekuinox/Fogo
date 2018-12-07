#include <string>
#include <algorithm>
#include <fbxsdk.h>
#include "FBX.h"
#include "../../Utility.h"

using namespace std;
using namespace Fogo::Graphics::DX12;
using namespace Fogo::Utility;

TCHAR	currentdirectory[MAX_PATH + 1];

constexpr float PI = 3.1415926535897932384626433832793f;

void ChangeAssetDirectory(TCHAR* assetdirectory) {

	GetCurrentDirectory(sizeof(currentdirectory), currentdirectory);
	printf("%s \n", currentdirectory);
	BOOL sts = SetCurrentDirectory(assetdirectory);
	if (sts == 0) {
		DWORD code = GetLastError();
		printf("カレントディレクトリ設定失敗\n %d", code);
	}
	TCHAR	tempdir[MAX_PATH + 1];
	GetCurrentDirectory(sizeof(tempdir), tempdir);
	printf("%s \n", tempdir);
}

void RestoreDirectory() {

	SetCurrentDirectory(currentdirectory);

}

void MatRotateZ(FBX::Matrix4x4& mat, float angle) {
	float	cos;
	float	sin;

	cos = cosf((angle*PI) / 180.0f);
	sin = sinf((angle*PI) / 180.0f);

	mat.m2[0][0] = cos;
	mat.m2[0][1] = sin;
	mat.m2[0][2] = 0.0f;
	mat.m2[0][3] = 0.0f;

	mat.m2[1][0] = -sin;
	mat.m2[1][1] = cos;
	mat.m2[1][2] = 0.0f;
	mat.m2[1][3] = 0.0f;

	mat.m2[2][0] = 0.0f;
	mat.m2[2][1] = 0.0f;
	mat.m2[2][2] = 1.0f;
	mat.m2[2][3] = 0.0f;

	mat.m2[3][0] = 0.0f;
	mat.m2[3][1] = 0.0f;
	mat.m2[3][2] = 0.0f;
	mat.m2[3][3] = 1.0f;
}

void MatRotateY(FBX::Matrix4x4& mat, float angle) {
	float	cos;
	float	sin;

	cos = cosf((angle*PI) / 180.0f);
	sin = sinf((angle*PI) / 180.0f);

	mat.m2[0][0] = cos;
	mat.m2[0][1] = 0.0f;
	mat.m2[0][2] = sin;
	mat.m2[0][3] = 0.0f;

	mat.m2[1][0] = 0.0f;
	mat.m2[1][1] = 1.0f;
	mat.m2[1][2] = 0.0f;
	mat.m2[1][3] = 0.0f;

	mat.m2[2][0] = -sin;
	mat.m2[2][1] = 0.0f;
	mat.m2[2][2] = cos;
	mat.m2[2][3] = 0.0f;

	mat.m2[3][0] = 0.0f;
	mat.m2[3][1] = 0.0f;
	mat.m2[3][2] = 0.0f;
	mat.m2[3][3] = 1.0f;
}

void MulVectorMatrix(FBX::Vector3& vec, const FBX::Matrix4x4& mat) {

	FBX::Vector3 tmp;
	float w;

	tmp.x = vec.x * mat.m2[0][0] +
		vec.y * mat.m2[1][0] +
		vec.z * mat.m2[2][0] +
		1 * mat.m2[3][0];

	tmp.y = vec.x * mat.m2[0][1] +
		vec.y * mat.m2[1][1] +
		vec.z * mat.m2[2][1] +
		1 * mat.m2[3][1];

	tmp.z = vec.x * mat.m2[0][2] +
		vec.y * mat.m2[1][2] +
		vec.z * mat.m2[2][2] +
		1 * mat.m2[3][2];

	w = vec.x * mat.m2[0][3] +
		vec.y * mat.m2[1][3] +
		vec.z * mat.m2[2][3] +
		1 * mat.m2[3][3];

	vec = tmp;

}


void MulVectorMatrix(const FBX::Vector3& vec, const FBX::Matrix4x4& mat, FBX::Vector3& ans) {

	FBX::Vector3 tmp;
	float w;

	tmp.x = vec.x * mat.m2[0][0] +
		vec.y * mat.m2[1][0] +
		vec.z * mat.m2[2][0] +
		1 * mat.m2[3][0];

	tmp.y = vec.x * mat.m2[0][1] +
		vec.y * mat.m2[1][1] +
		vec.z * mat.m2[2][1] +
		1 * mat.m2[3][1];

	tmp.z = vec.x * mat.m2[0][2] +
		vec.y * mat.m2[1][2] +
		vec.z * mat.m2[2][2] +
		1 * mat.m2[3][2];

	w = vec.x * mat.m2[0][3] +
		vec.y * mat.m2[1][3] +
		vec.z * mat.m2[2][3] +
		1 * mat.m2[3][3];

	ans = tmp;

}



//-------------------------------------------------
//	概要：	４行４列の行列の掛算を行う
//	入力：	mat1	行列１
//			mat2	行列２
//			ans		行列１×行列２
//-------------------------------------------------
void MulMatrixMatrix(const FBX::Matrix4x4& mat1, const FBX::Matrix4x4& mat2, FBX::Matrix4x4& ans)
{
	FBX::Matrix4x4 temp;

	temp.m2[0][0] = mat1.m2[0][0] * mat2.m2[0][0] +
		mat1.m2[0][1] * mat2.m2[1][0] +
		mat1.m2[0][2] * mat2.m2[2][0] +
		mat1.m2[0][3] * mat2.m2[3][0];

	temp.m2[0][1] = mat1.m2[0][0] * mat2.m2[0][1] +
		mat1.m2[0][1] * mat2.m2[1][1] +
		mat1.m2[0][2] * mat2.m2[2][1] +
		mat1.m2[0][3] * mat2.m2[3][1];

	temp.m2[0][2] = mat1.m2[0][0] * mat2.m2[0][2] +
		mat1.m2[0][1] * mat2.m2[1][2] +
		mat1.m2[0][2] * mat2.m2[2][2] +
		mat1.m2[0][3] * mat2.m2[3][2];

	temp.m2[0][3] = mat1.m2[0][0] * mat2.m2[0][3] +
		mat1.m2[0][1] * mat2.m2[1][3] +
		mat1.m2[0][2] * mat2.m2[2][3] +
		mat1.m2[0][3] * mat2.m2[3][3];


	temp.m2[1][0] = mat1.m2[1][0] * mat2.m2[0][0] +
		mat1.m2[1][1] * mat2.m2[1][0] +
		mat1.m2[1][2] * mat2.m2[2][0] +
		mat1.m2[1][3] * mat2.m2[3][0];

	temp.m2[1][1] = mat1.m2[1][0] * mat2.m2[0][1] +
		mat1.m2[1][1] * mat2.m2[1][1] +
		mat1.m2[1][2] * mat2.m2[2][1] +
		mat1.m2[1][3] * mat2.m2[3][1];

	temp.m2[1][2] = mat1.m2[1][0] * mat2.m2[0][2] +
		mat1.m2[1][1] * mat2.m2[1][2] +
		mat1.m2[1][2] * mat2.m2[2][2] +
		mat1.m2[1][3] * mat2.m2[3][2];

	temp.m2[1][3] = mat1.m2[1][0] * mat2.m2[0][3] +
		mat1.m2[1][1] * mat2.m2[1][3] +
		mat1.m2[1][2] * mat2.m2[2][3] +
		mat1.m2[1][3] * mat2.m2[3][3];

	temp.m2[2][0] = mat1.m2[2][0] * mat2.m2[0][0] +
		mat1.m2[2][1] * mat2.m2[1][0] +
		mat1.m2[2][2] * mat2.m2[2][0] +
		mat1.m2[2][3] * mat2.m2[3][0];

	temp.m2[2][1] = mat1.m2[2][0] * mat2.m2[0][1] +
		mat1.m2[2][1] * mat2.m2[1][1] +
		mat1.m2[2][2] * mat2.m2[2][1] +
		mat1.m2[2][3] * mat2.m2[3][1];

	temp.m2[2][2] = mat1.m2[2][0] * mat2.m2[0][2] +
		mat1.m2[2][1] * mat2.m2[1][2] +
		mat1.m2[2][2] * mat2.m2[2][2] +
		mat1.m2[2][3] * mat2.m2[3][2];

	temp.m2[2][3] = mat1.m2[2][0] * mat2.m2[0][3] +
		mat1.m2[2][1] * mat2.m2[1][3] +
		mat1.m2[2][2] * mat2.m2[2][3] +
		mat1.m2[2][3] * mat2.m2[3][3];

	temp.m2[3][0] = mat1.m2[3][0] * mat2.m2[0][0] +
		mat1.m2[3][1] * mat2.m2[1][0] +
		mat1.m2[3][2] * mat2.m2[2][0] +
		mat1.m2[3][3] * mat2.m2[3][0];

	temp.m2[3][1] = mat1.m2[3][0] * mat2.m2[0][1] +
		mat1.m2[3][1] * mat2.m2[1][1] +
		mat1.m2[3][2] * mat2.m2[2][1] +
		mat1.m2[3][3] * mat2.m2[3][1];

	temp.m2[3][2] = mat1.m2[3][0] * mat2.m2[0][2] +
		mat1.m2[3][1] * mat2.m2[1][2] +
		mat1.m2[3][2] * mat2.m2[2][2] +
		mat1.m2[3][3] * mat2.m2[3][2];

	temp.m2[3][3] = mat1.m2[3][0] * mat2.m2[0][3] +
		mat1.m2[3][1] * mat2.m2[1][3] +
		mat1.m2[3][2] * mat2.m2[2][3] +
		mat1.m2[3][3] * mat2.m2[3][3];

	ans = temp;

}


void GetWeight(FbxMesh* mesh,								// ＦＢＸＭＥＳＨ
	const std::vector<int>& indexList,						// 面の構成情報（頂点インデックス番号）
	std::vector<FBX::BoneWeight>& boneWeightList,			// ウェイト値格納ＶＥＣＴＯＲ
	std::vector<std::string>& boneNodeNameList,				// ボーンノード名格納ＶＥＣＴＯＲ	
	std::vector<FBX::Matrix4x4>& invBaseposeMatrixList)			// 初期姿勢の逆行列格納ＶＥＣＴＯＲ
{
	// メッシュ内のスキン情報数を取得する
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount == 0)
	{
		return;
	}

	// スキンが1つにしか対応しない
	ExecOrFail(skinCount <= 1);

	int controlPointsCount = mesh->GetControlPointsCount();		// メッシュの頂点数を取得
//	using TmpWeight = std::pair<int, float>;					// std::pair *=> 2つのものの入れ物(vs2010でエラー)
																// using TmpWeightでint とfloatの入れ物をＴｍｐＷｅｉｇｈｔと定義したことになる
	std::vector<std::vector<std::pair<int, float>>> tmpBoneWeightList(controlPointsCount);	// ボーンインデックス値とウェイト値をペアにしたＶＥＣＴＯＲ

	// ＦｂｘＭｅｓｈからスキン情報を取り出す
	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
	int clusterCount = skin->GetClusterCount();	// スキンからクラスタ（ボーン）数を取得

	// クラスター（ボーンの事）の数分ループ
	for (int i = 0; i < clusterCount; ++i)
	{
		FbxCluster* cluster = skin->GetCluster(i);					// i番目のクラスタ（ボーン）を取得

		// eNormalizeしか対応しない
		ExecOrFail(cluster->GetLinkMode() == FbxCluster::eNormalize);	// 正規化されていないウェイト値は対応していない

		boneNodeNameList.push_back(cluster->GetLink()->GetName());	// ボーン名を格納

		int indexCount = cluster->GetControlPointIndicesCount();	// ボーンに所属する頂点インデックス数を取得
		int* indices = cluster->GetControlPointIndices();			// 頂点インデックス配列を取得
		double* weights = cluster->GetControlPointWeights();		// ウェイト値配列を取得

		// ボーンに所属する頂点インデックス数分ループする
		for (int j = 0; j < indexCount; ++j)
		{
			int controlPointIndex = indices[j];		// このボーンに影響を受けるj番目の頂点インデックス番号を取得
			tmpBoneWeightList[controlPointIndex].push_back(std::pair<int, float>(i, static_cast<float>(weights[j])));
			// 頂点インデックス番号のＶＥＣＴＯＲ位置に（i番目のボーンとそのウエイト値）
		}

		// ペースポーズの逆行列を作成しておく
		FBX::Matrix4x4 invBaseposeMatrix;

		FbxMatrix baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();		// 逆行列を計算する

		// FbxMatrix->Matrix4x4へ
		auto baseposeMatrixPtr = (double*)baseposeMatrix;
		for (int j = 0; j < 16; ++j)
		{
			invBaseposeMatrix.m[j] = static_cast<float>(baseposeMatrixPtr[j]);	// double->float
		}

		invBaseposeMatrixList.push_back(invBaseposeMatrix);			// i番目のボーンの初期姿勢を示す逆行列
	}

	// コントロールポイントに対応したウェイトを作成
	std::vector<FBX::BoneWeight> boneWeightListControlPoints;

	for (unsigned int j = 0; j < tmpBoneWeightList.size(); j++) {
		// ｊ番目の頂点のウェイト値を取得
		std::vector<std::pair<int, float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// ウェイト値の大きさでソート
		std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
			[](const std::pair<int, float>& weightA, const std::pair<int, float>& weightB) { return weightA.second > weightB.second; }
		);

		// 1頂点に4つより多くウェイトが割り振られているなら影響が少ないものは無視する
		while (tmpBoneWeight.size() > 4)
		{
			tmpBoneWeight.pop_back();
		}

		// 4つに満たない場合はダミーを挿入
		while (tmpBoneWeight.size() < 4)
		{
			tmpBoneWeight.push_back(std::pair<int, float>(0, 0.0f));
		}

		FBX::BoneWeight weight;
		float total = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			weight.boneIndex[i] = tmpBoneWeight[i].first;
			weight.boneWeight.v[i] = tmpBoneWeight[i].second;

			total += tmpBoneWeight[i].second;
		}

		// ウェイトの正規化
		for (int i = 0; i < 4; ++i)
		{
			weight.boneWeight.v[i] /= total;
		}

		boneWeightListControlPoints.push_back(weight);


	}

	// インデックスで展開
	for (unsigned int j = 0; j < indexList.size(); j++) {
		int index = indexList[j];				// j番目の頂点インデックスを取得
		boneWeightList.push_back(boneWeightListControlPoints[index]);	// j番目の頂点のボーン番号とウエイト値をセット
	}
}

std::vector<FBX::Vector2> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo)
{
	std::vector<FBX::Vector2> uvList;

	int elementCount = mesh->GetElementUVCount();		// 何個のＵＶ情報がセットされているか
	if (uvNo + 1 > elementCount)						// 今回は１個にのみ対応
	{
		return uvList;
	}

	FbxGeometryElementUV* element = mesh->GetElementUV(uvNo);					// UVセットを取得
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// マッピングモードを取得
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// リファレンスモードを取得	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// ＵＶ情報を格納したＶＥＣＴＯＲ配列のインデックス配列を取得
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// ＵＶ値配列を取得

	// eDirctかeIndexDirectのみ対応
	ExecOrFail((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// 頂点インデックス数分ｖｅｃｔｏｒ容量を確保
	uvList.reserve(indexList.size());

	// 頂点に対応して格納されている場合
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// 頂点座標でマッピング
		for (unsigned int i = 0; i < indexList.size(); i++)
		{
			int index = indexList[i];			// 面の構成情報配列から頂点インデックス番号を取得

			// リファレンスモードを判定
			int uvIndex;
			if (referenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
				uvIndex = index;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
			}
			else {													// eIndexToDirectの場合
				uvIndex = indexArray.GetAt(index);				// 頂点座標インデックスに対応したＵＶ情報インデックスを取得
			}

			FbxVector2 uv = directArray.GetAt(uvIndex);		// uv値をdouble型で取得
			uvList.push_back(FBX::Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));		// float値として格納
		}
	}
	// 面の構成情報に対応して格納されている場合
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// ポリゴンバーテックス（面の構成情報のインデックス）でマッピング
		int indexByPolygonVertex = 0;						// 面の構成情報インデックス配列のインデックス
		int polygonCount = mesh->GetPolygonCount();			// メッシュのポリゴン数を取得
		for (int i = 0; i < polygonCount; ++i)				// ポリゴン数分ループ
		{
			int polygonSize = mesh->GetPolygonSize(i);		// ｉ番目のポリゴン頂点数を取得

			// ポリゴンの頂点数分ループ
			for (int j = 0; j < polygonSize; ++j)
			{
				// リファレンスモードの判定？
				int uvIndex;
				if (referenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
					uvIndex = indexByPolygonVertex;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
				}
				else {													// eIndexToDirectの場合
					uvIndex = indexArray.GetAt(indexByPolygonVertex);	// 面の構成情報インデックスに対応したＵＶ情報インデックスを取得
				}
				FbxVector2 uv = directArray.GetAt(uvIndex);

				uvList.push_back(FBX::Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));	// ｆｌｏａｔ値として格納

				++indexByPolygonVertex;						// 頂点インデックスをインクリメント
			}
		}
	}
	else
	{
		// それ以外のマッピングモードには対応しない
		ExecOrFail(false);
	}

	return uvList;
}

std::vector<FBX::Vector3> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList) {

	int elementCount = mesh->GetElementNormalCount();			// 何個の法線情報がセットされているか

	ExecOrFail(elementCount == 1);					// １個の法線情報にしか対応しない

	FbxGeometryElementNormal* element = mesh->GetElementNormal(0);	// ０番目の法線セットを取得
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// マッピングモード取得
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// リファレンスモード取得
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// 法線情報を格納したｖｅｃｔｏｒ配列のインデックス値配列を取得 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// 法線情報を格納したｖｅｃｔｏｒ配列を取得

	// eDirectか　eIndexDirectのみ対応
	ExecOrFail((referrenceMode == FbxGeometryElement::eDirect) || (referrenceMode == FbxGeometryElement::eIndexToDirect));

	// 法線情報ＶＥＣＴＯＲ
	std::vector<FBX::Vector3> normalList;
	normalList.reserve(indexList.size());		// 頂点インデックス数分確保

	// 頂点座標でマッピングされている場合
	if (mappingmode == FbxGeometryElement::eByControlPoint) {
		for (unsigned int i = 0; i < indexList.size(); i++) {
			int index = indexList[i];		// 面の構成情報配列のi番目の頂点インデックスを取得
			int normalIndex;				// 法線情報のインデックス
			// リファレンスモードを判定
			if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
				normalIndex = index;								// 頂点インデックスの位置に対応して保存されている
			}
			else {													// eIndexToDirectの場合
				normalIndex = indexArray.GetAt(index);				// 頂点座標インデックスに対応した法線情報VECTORのインデックスを取得
			}
			FbxVector4 normal = directArray.GetAt(normalIndex);		// 法線情報を取得
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.push_back(FBX::Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));			// 同次座標へ
				}
				else {
					normalList.push_back(FBX::Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));		// 同次座標へ
				}
			}
		}
	}
	else if (mappingmode == FbxGeometryElement::eByPolygonVertex) {
		// 頂点インデックス(面の構成情報)でマッピングされている場合
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// ポリゴン数を取得
		for (int i = 0; i < polygonCount; ++i) {
			int polygonSize = mesh->GetPolygonSize(i);				// 頂点数を取得
			int normalIndex;
			for (int j = 0; j < polygonSize; ++j) {
				// リファレンスモードを判定
				if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirectの場合
					normalIndex = indexByPolygonVertex;
				}
				else {
					normalIndex = indexArray.GetAt(indexByPolygonVertex);	// eIndexToDirectの場合
				}
				FbxVector4 normal = directArray.GetAt(normalIndex);		// 法線情報を取得
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0) {
						normalList.push_back(FBX::Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));		// 同次座標へ
					}
					else {
						normalList.push_back(FBX::Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));	// 同次座標へ
					}
				}
				++indexByPolygonVertex;
			}
		}
	}
	else {
		ExecOrFail(false);
	}

	return normalList;
}

std::vector<FBX::Vector3> GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList) {
	//　頂点座標を取得
	std::vector<FBX::Vector3> positionList;					//	頂点座標格納ＶＥＣＴＯＲ 
	positionList.reserve(indexList.size());				//  

	for (unsigned int i = 0; i < indexList.size(); i++) {
		int index = indexList[i];						// i番目の頂点情報（面の構成上情報順に並んでいる）を取得
		FbxVector4 Position = mesh->GetControlPointAt(index);		// index番目の頂点座標を取得
		if (Position[3] != 0.0) {
			if (Position[3] == 1.0) {
				positionList.push_back(FBX::Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));			// 同次座標化
			}
			else {
				positionList.push_back(FBX::Vector3(static_cast<float>(Position[0] / Position[3]), static_cast<float>(Position[1] / Position[3]), static_cast<float>(Position[2] / Position[3])));		// 同次座標化
			}
		}
		else {
			positionList.push_back(FBX::Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));				// Ｗを無視して格納
		}
	}
	return positionList;
}

std::vector<int> GetIndexList(FbxMesh* mesh) {

	int polygonCount = mesh->GetPolygonCount();			// 三角形の数を取得
	std::vector<int> indexList;							// 頂点インデックス（面の構成情報）
	indexList.reserve(polygonCount * 3);				// 3角形の数×３

	// 面の構成情報を取得する
	for (int i = 0; i < polygonCount; i++) {
		indexList.push_back(mesh->GetPolygonVertex(i, 0));		// i番目の三角形の０番目の頂点インデックスを取得
		indexList.push_back(mesh->GetPolygonVertex(i, 1));		// i番目の三角形の１番目の頂点インデックスを取得
		indexList.push_back(mesh->GetPolygonVertex(i, 2));		// i番目の三角形の２番目の頂点インデックスを取得
	}
	return indexList;
}

FBX::Mesh ParseMesh(FbxMesh* mesh) {

	ExecOrFail(mesh != nullptr);

	FbxNode* fbxnode = mesh->GetNode();					// ＦＢＸノードを取得

	// マテリアルが1のものしか対応しない
	ExecOrFail(fbxnode->GetMaterialCount() == 1);

	FBX::Mesh modelMesh;								// 最終的に保存されるメッシュ情報（この関数の出力値）

	// ワンスキンの数を取得
	modelMesh.skincount = mesh->GetDeformerCount(FbxDeformer::eSkin);		// ワンスキン情報の数をセット
	modelMesh.nodeName = fbxnode->GetName();								// ＦＢＸノード名をセット
	modelMesh.materialName = fbxnode->GetMaterial(0)->GetName();			// マテリアル名をセット

//	printf("mesh:%s ,%s \n", modelMesh.nodeName.c_str(), modelMesh.materialName.c_str());

	// ベースポーズの逆行列を作成しておく(メッシュの)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	double* baseposeMatrixPtr = (double*)baseposeMatrix;

	for (int i = 0; i < 16; ++i) {
		modelMesh.invMeshBaseposeMatrix.m[i] = static_cast<float>(baseposeMatrixPtr[i]);
	}

	// インデックスＶＥＣＴＯＲ取得
	std::vector<int> indexList = GetIndexList(mesh);

	// 頂点データ取得
	std::vector<FBX::Vector3> positionList = GetPositionList(mesh, indexList);		// 頂点座標を取得（面の構成情報に合わせる）
	std::vector<FBX::Vector3> normalList = GetNormalList(mesh, indexList);			// 法線ベクトルを取得（面の構成情報に合わせる）
	std::vector<FBX::Vector2> uv0List = GetUVList(mesh, indexList, 0);				// UV座標を取得（面の構成情報に合わせる）

	// ウエイト値を取得する
	std::vector<FBX::BoneWeight> boneWeightList;
	GetWeight(mesh,											// メッシュ（入力）
		indexList,										// 面の構成情報（入力）
		boneWeightList,									// ボーン情報リスト（出力値）（ボーン情報へのインデックス値とウェイト値）
		modelMesh.boneNodeNameList,						// メッシュ内のボーン名をＶＥＣＴＯＲに保存（出力）
		modelMesh.invBoneBaseposeMatrixList);			// ボーンオフセット行列をＶＥＣＴＯＲに保存（）

// 念のためサイズチェック
	ExecOrFail(indexList.size() == positionList.size());
	ExecOrFail(indexList.size() == normalList.size());
	ExecOrFail(indexList.size() == uv0List.size());
	ExecOrFail((indexList.size() == boneWeightList.size()) || (boneWeightList.size() == 0));

	// テンポラリの頂点情報（ＭｏｄｅｌＶｅｒｔｅｘ）を作る
	std::vector<FBX::Vertex> tmpmodelVertexList;
	tmpmodelVertexList.reserve(indexList.size());		// 頂点インデックスサイズ分容量を確保

	// 頂点インデックス数分（面の構成情報順）ループする
	for (unsigned int i = 0; i < indexList.size(); ++i)
	{
		FBX::Vertex vertex;
		vertex.position = positionList[i];			// 頂点情報
		vertex.normal = normalList[i];				// 法線情報

		if (uv0List.size() == 0) {
			vertex.uv0 = FBX::Vector2(0.0f, 0.0f);
		}
		else {
			vertex.uv0 = uv0List[i];
		}

		// ボーンウェイト値が存在するか？
		if (boneWeightList.size() > 0)
		{
			// ボーンのインデックス番号をセット
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = boneWeightList[i].boneIndex[j];
			}
			// ウェイト値をセット
			vertex.boneWeight = boneWeightList[i].boneWeight;
		}
		else
		{
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = 0;
			}
			vertex.boneWeight = FBX::Vector4(1.0, 0.0, 0.0, 0.0);
		}

		tmpmodelVertexList.push_back(vertex);			// 
	}

	// 重複頂点を除いてインデックス作成
	modelMesh.vertexList.reserve(tmpmodelVertexList.size());		// 頂点情報数分エリアを確保する

	modelMesh.indexList.reserve(indexList.size());		// 面の構成情報数分エリアを確保する

	// ここで重複している頂点データを除き、頂点データリストとインデックスリストを作り直している
	// 重複した頂点を含んだ頂点数分ループしている(面の構成情報を元に抜き出した頂点情報)
	for (unsigned int i = 0; i < tmpmodelVertexList.size(); i++)
	{
		FBX::Vertex & vertex = tmpmodelVertexList[i];
		// 重複しているか？
		// modelMesh.vertexListは、最初空でだんだん登録されていく（重複していない頂点情報として）
		auto it = std::find(modelMesh.vertexList.begin(), modelMesh.vertexList.end(), vertex);
		if (it == modelMesh.vertexList.end())
		{
			// 重複していない
			modelMesh.indexList.push_back(modelMesh.vertexList.size());	// 頂点インデックスを格納
			modelMesh.vertexList.push_back(vertex);					// 頂点情報を格納
		}
		else
		{
			// 重複している
			auto index = std::distance(modelMesh.vertexList.begin(), it);	// 先頭から現イテレータ（重複頂点した先頭データを指し示している）までのインデックス番号を取得
			modelMesh.indexList.push_back(index);// インデックス番号（重複頂点した先頭データを指し示している）をインデックスリストにセット
		}
	}

	return modelMesh;

}

FBX::Material ParseMaterial(FbxSurfaceMaterial* material) {
	FBX::Material modelMaterial;
	modelMaterial.materialName = material->GetName();

	printf("material name :%s \n", modelMaterial.materialName.c_str());

	enum eMATERIAL_TYPE {
		MATERIAL_LAMBERT,
		MATERIAL_PHONG
	};

	eMATERIAL_TYPE material_type;

	// マテリアル情報取得
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
		// lambert
		material_type = MATERIAL_LAMBERT;
		FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)material;
		modelMaterial.Diffuse.v[0] = (float)pLambert->Diffuse.Get()[0];
		modelMaterial.Diffuse.v[1] = (float)pLambert->Diffuse.Get()[1];
		modelMaterial.Diffuse.v[2] = (float)pLambert->Diffuse.Get()[2];
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = (float)pLambert->Ambient.Get()[0];
		modelMaterial.Ambient.v[1] = (float)pLambert->Ambient.Get()[1];
		modelMaterial.Ambient.v[2] = (float)pLambert->Ambient.Get()[2];

		modelMaterial.Speqular.v[0] = 0.0f;
		modelMaterial.Speqular.v[1] = 0.0f;
		modelMaterial.Speqular.v[2] = 0.0f;
		modelMaterial.Speqular.v[3] = 1.0f;

		modelMaterial.Power = 1.0f;
	}
	else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
		// phong
		material_type = MATERIAL_PHONG;
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)(material);
		modelMaterial.Diffuse.v[0] = (float)pPhong->Diffuse.Get()[0];
		modelMaterial.Diffuse.v[1] = (float)pPhong->Diffuse.Get()[1];
		modelMaterial.Diffuse.v[2] = (float)pPhong->Diffuse.Get()[2];
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = pPhong->Ambient.Get()[0];
		modelMaterial.Ambient.v[1] = pPhong->Ambient.Get()[1];
		modelMaterial.Ambient.v[2] = pPhong->Ambient.Get()[2];
		modelMaterial.Ambient.v[3] = 1.0f;

		modelMaterial.Speqular.v[0] = pPhong->Specular.Get()[0];
		modelMaterial.Speqular.v[1] = pPhong->Specular.Get()[1];
		modelMaterial.Speqular.v[2] = pPhong->Specular.Get()[2];
		modelMaterial.Speqular.v[3] = pPhong->Specular.Get()[3];

		modelMaterial.Power = pPhong->Shininess;
	}
	else {
		modelMaterial.Diffuse.v[0] = 1.0f;
		modelMaterial.Diffuse.v[1] = 1.0f;
		modelMaterial.Diffuse.v[2] = 1.0f;
		modelMaterial.Diffuse.v[3] = 1.0f;

		modelMaterial.Ambient.v[0] = 1.0f;
		modelMaterial.Ambient.v[1] = 1.0f;
		modelMaterial.Ambient.v[2] = 1.0f;
		modelMaterial.Ambient.v[3] = 1.0f;

		modelMaterial.Speqular.v[0] = 0.0f;
		modelMaterial.Speqular.v[1] = 0.0f;
		modelMaterial.Speqular.v[2] = 0.0f;
		modelMaterial.Speqular.v[3] = 1.0f;

		modelMaterial.Power = 0.0f;
	}

	printf("material diffuse:%f %f %f \n", modelMaterial.Diffuse.v[0], modelMaterial.Diffuse.v[1], modelMaterial.Diffuse.v[2]);
	printf("material specular:%f %f %f \n", modelMaterial.Speqular.v[0], modelMaterial.Speqular.v[1], modelMaterial.Speqular.v[2]);

	const FbxProperty propertyb = material->FindProperty(FbxSurfaceMaterial::sEmissive);
	const FbxProperty Factory = material->FindProperty(FbxSurfaceMaterial::sEmissive);

	// テクスチャを取得
	auto implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX);
	ExecOrFail(implementation != nullptr);

	if (implementation != nullptr) {
		auto rootTable = implementation->GetRootTable();
		auto entryCount = rootTable->GetEntryCount();

		for (unsigned int i = 0; i < entryCount; ++i) {
			auto entry = rootTable->GetEntry(i);
			auto fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
			if (!fbxProperty.IsValid()) {
				fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
			}

			auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
			if (textureCount > 0) {
				std::string src = entry.GetSource();

				for (int j = 0; j < fbxProperty.GetSrcObjectCount<FbxFileTexture>(); ++j) {

					auto tex = fbxProperty.GetSrcObject<FbxFileTexture>(j);
					std::string texName = tex->GetFileName();
					texName = texName.substr(texName.find_last_of('/') + 1);

					if (src == "Maya|DiffuseTexture") {
						modelMaterial.diffuseTextureName = texName;
					}
					else if (src == "Maya|NormalTexture") {
						modelMaterial.normalTextureName = texName;
					}
					else if (src == "Maya|SpecularTexture") {
						modelMaterial.specularTextureName = texName;
					}
					else if (src == "Maya|FalloffTexture") {
						modelMaterial.falloffTextureName = texName;
					}
					else if (src == "Maya|ReflectionMapTexture") {
						modelMaterial.reflectionMapTextureName = texName;
					}
				}
			}
		}

		//	printf("diffuseTexture: %s\n",modelMaterial.diffuseTextureName.c_str());
		//	printf("normalTexture: %s\n", modelMaterial.normalTextureName.c_str());
		//	printf("specularTexture: %s\n", modelMaterial.specularTextureName.c_str());
		//	printf("falloffTexture: %s\n", modelMaterial.falloffTextureName.c_str());
		//	printf("reflectionMapTexture: %s\n", modelMaterial.reflectionMapTextureName.c_str());


		//	FILE* fp;
		//	fopen_s(&fp,"texlist.txt","a");

		//	fprintf(fp,"diffuseTexture: %s\n", modelMaterial.diffuseTextureName.c_str());

		//	fclose(fp);

	}

	return modelMaterial;
}

FBX::FBX(const char * modelFile, const char * animationFile)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// FbxManager生成
	manager = FbxManager::Create();
	if (!manager) exit(1);

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	// Scene生成
	scene = FbxScene::Create(manager, "originalScene");
	if (!scene) exit(1);

	// Importer生成
	importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize(modelFile, -1, manager->GetIOSettings())) exit(1);

	// FBXをパースしてSceneに読み込む
	if (!importer->Import(scene)) exit(1);

	// インポーター解放
	importer->Destroy();

	// あらかじめポリゴンを全て３角形化しておく
	FbxGeometryConverter geometryConverter(manager);
	geometryConverter.Triangulate(scene, true);

	// シーンに含まれるメッシュの解析
	auto meshCount = scene->GetMemberCount<FbxMesh>();
	printf("メッシュの数 %d \n", meshCount);

	int nodeCount = scene->GetNodeCount();
	printf("ノードの数 %d \n", nodeCount);

	// ノード名からノードIDを取得できるように辞書に登録
	for (auto i = 0; i < nodeCount; i++) {
		FbxNode* fbxNode = scene->GetNode(i);
		nodeIdDictionary.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	// ノードの中身を確認
	for (std::map<std::string, int>::iterator it = nodeIdDictionary.begin(); it != nodeIdDictionary.end(); ++it) {
		//		std::cout << "key = " << (*it).first << ",value = " << (*it).second << std::endl;
	}

	// シーンに含まれるマテリアルの解析
	static int materialCount = scene->GetMaterialCount();
	printf("マテリアルの数 %d \n", materialCount);

	materialList.reserve(materialCount);
	for (int i = 0; i < materialCount; ++i) {
		FbxSurfaceMaterial* fbxMaterial = scene->GetMaterial(i);


		Material modelMaterial = ParseMaterial(fbxMaterial);
		materialList.push_back(modelMaterial);
		materialIdDictionary.insert(std::pair<std::string, int>(modelMaterial.materialName, i));
	}

	// シーンに含まれるメッシュの解析
	meshCount = scene->GetMemberCount<FbxMesh>();
	meshList.reserve(meshCount);
	// ts	printf("meshCount: %d\n", meshCount);

	for (int i = 0; i < meshCount; ++i) {
		FbxMesh* fbxMesh = scene->GetMember<FbxMesh>(i);
		meshList.push_back(ParseMesh(fbxMesh));
	}

	// アニメーション読み込み
	if (animationFile) loadAnimation(animationFile);
}

FBX::~FBX()
{
	if (scene)
	{
		scene->Destroy();
		scene = nullptr;
	}

	if (fbxSceneAnimation) {
		fbxSceneAnimation->Destroy();
		fbxSceneAnimation = nullptr;
	}

	if (manager)
	{
		manager->Destroy();
		manager = nullptr;
	}

}

void FBX::getBoneMatrix(INT64 frame, int meshId, Matrix4x4 * out_matrixList, unsigned int matrixCount)
{
	Mesh & modelMesh = meshList[meshId];				// ここで落ちた　t.s 2014/08/09(修正完了)

	if (fbxSceneAnimation == nullptr) {
		for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); ++i) {
			out_matrixList[i] = matrix4x4Identity;
		}
		return;
	}


	if (modelMesh.boneNodeNameList.empty())				// ボーン名リストが空なら
	{
		out_matrixList[0] = matrix4x4Identity;
		return;
	}

	ExecOrFail(modelMesh.boneNodeNameList.size() <= matrixCount);	// ボーン数が配列数より多い場合強制終了

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);	// 指定フレーム数に応じた経過時間をセット

	// このメッシュに含まれているボーン数分ループ
	for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); ++i)
	{
		std::string& boneNodeName = modelMesh.boneNodeNameList[i];			// ボーンノード名取得
		int boneNodeId = nodeIdDictionaryAnimation.at(boneNodeName);		// ボーンノード名からボーンノードＩＤ取得
		FbxNode* boneNode = fbxSceneAnimation->GetNode(boneNodeId);			// アニメーションノード取得

		FbxMatrix boneMatrix = boneNode->EvaluateGlobalTransform(time);		// 経過時間に応じた姿勢をあらわすボーン行列を取得
		Matrix4x4& out_matrix = out_matrixList[i];

		// FbxMatrix（doubleの配列）からMatrix4x4(floatの配列)へ変換
		double* boneMatrixPtr = (double*)boneMatrix;
		for (int j = 0; j < 16; ++j)
		{
			out_matrix.m[j] = static_cast<float>(boneMatrixPtr[j]);			// double->float
		}

		MulMatrixMatrix(modelMesh.invBoneBaseposeMatrixList[i], out_matrix, out_matrix);		// ボーンオフセット行列＊ボーン行列
	}
}

void FBX::getMeshMatrix(INT64 frame, int meshId, Matrix4x4 & out_matrix)
{
	Mesh & modelMesh = meshList[meshId];

	if (fbxSceneAnimation == nullptr) {
		out_matrix = matrix4x4Identity;
		return;
	}

	std::map<std::string, int>::iterator it = nodeIdDictionaryAnimation.find(modelMesh.nodeName);		// メッシュのノード名からノードＩＤを取得
	// 見つからなかった場合
	if (it == nodeIdDictionaryAnimation.end())
	{
		out_matrix = matrix4x4Identity;
		return;
	}

	int meshNodeId = it->second;											// ノードＩＤ取得
	FbxNode* meshNode = fbxSceneAnimation->GetNode(meshNodeId);				// FBXノードを取得

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);		// 指定フレームに応じた経過時間をセット

	FbxMatrix meshMatrix = meshNode->EvaluateGlobalTransform(time);			// 経過時間に応じた姿勢をあらわす行列を取得

	// FbxMatrix（doubleの配列）からMatrix4x4(floatの配列)へ変換
	const auto meshMatrixPtr = static_cast<double*>(meshMatrix);
	for (int i = 0; i < 16; ++i)
	{
		out_matrix.m[i] = static_cast<float>(meshMatrixPtr[i]);				// double->float
	}

	MulMatrixMatrix(modelMesh.invMeshBaseposeMatrix, out_matrix, out_matrix);		// オフセット行列＊メッシュ姿勢行列
}

void FBX::loadAnimation(const char * animationfilename)
{
	// すでにアニメーションを読み込んでいたら
	if (fbxSceneAnimation) {
		fbxSceneAnimation->Destroy();
		fbxSceneAnimation = nullptr;
		nodeIdDictionaryAnimation.clear();
	}

	// アニメーション情報取得
	// Importer生成
	importer = FbxImporter::Create(manager, "");

	if (!importer->Initialize(animationfilename, -1, manager->GetIOSettings())) exit(1);

	// animationScene生成
	fbxSceneAnimation = FbxScene::Create(manager, "animationScene");
	if (!fbxSceneAnimation) exit(1);

	// FBXをパースしてfbxSceneAnimationに読み込む
	if (!importer->Import(fbxSceneAnimation)) exit(1);

	// アニメーション数取得
	ExecOrFail(importer->GetAnimStackCount() == 1);								// １アニメーションだけに対応

	FbxTakeInfo* takeInfo = importer->GetTakeInfo(0);				// ０番目のＴＡＫＥを取得

	FbxTime importOffset = takeInfo->mImportOffset;					// オフセット時間
	FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();		// 開始時刻を取得
	FbxTime stopTime = takeInfo->mLocalTimeSpan.GetStop();			// 終了時刻を取得

	// アニメーション開始時間時間／１フレームの時間で開始フレームを求めている
	animationStartFrame = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	// アニメーション終了時間／１フレームの時間の時間で終了フレームを求めている
	animationEndFrame = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	importer->Destroy();

	// ノード名からノードIDを取得できるように辞書に登録
	int nodeCount = fbxSceneAnimation->GetNodeCount();
	printf("animationNodeCount: %d\n", nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		auto fbxNode = fbxSceneAnimation->GetNode(i);
		nodeIdDictionaryAnimation.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	// デバッグ用
//	FILE* fp2;
//	fopen_s(&fp2, "animationnode.txt", "w");
//	for (std::map<std::string, int>::iterator it = nodeIdDictionaryAnimation.begin(); it != nodeIdDictionaryAnimation.end(); ++it){
//		fprintf(fp2, "%s (%d)\n", (*it).first.c_str(), (*it).second);
//	}

//	fclose(fp2);


}

std::vector<FBX::Mesh>& FBX::getModelMeshContainer()
{
	return meshList;
}

FBX::Material &FBX::getMaterial(std::string materialname)
{
	// マテリアル名でインデックスをサーチ
	return materialList[(*materialIdDictionary.find(materialname)).second];
}

bool FBX::isAnimationEnd(INT64 frame)
{
	return fbxSceneAnimation ? frame > animationEndFrame : false;
}
