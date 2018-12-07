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
		printf("�J�����g�f�B���N�g���ݒ莸�s\n %d", code);
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
//	�T�v�F	�S�s�S��̍s��̊|�Z���s��
//	���́F	mat1	�s��P
//			mat2	�s��Q
//			ans		�s��P�~�s��Q
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


void GetWeight(FbxMesh* mesh,								// �e�a�w�l�d�r�g
	const std::vector<int>& indexList,						// �ʂ̍\�����i���_�C���f�b�N�X�ԍ��j
	std::vector<FBX::BoneWeight>& boneWeightList,			// �E�F�C�g�l�i�[�u�d�b�s�n�q
	std::vector<std::string>& boneNodeNameList,				// �{�[���m�[�h���i�[�u�d�b�s�n�q	
	std::vector<FBX::Matrix4x4>& invBaseposeMatrixList)			// �����p���̋t�s��i�[�u�d�b�s�n�q
{
	// ���b�V�����̃X�L����񐔂��擾����
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount == 0)
	{
		return;
	}

	// �X�L����1�ɂ����Ή����Ȃ�
	ExecOrFail(skinCount <= 1);

	int controlPointsCount = mesh->GetControlPointsCount();		// ���b�V���̒��_�����擾
//	using TmpWeight = std::pair<int, float>;					// std::pair *=> 2�̂��̂̓��ꕨ(vs2010�ŃG���[)
																// using TmpWeight��int ��float�̓��ꕨ���s�����v�����������ƒ�`�������ƂɂȂ�
	std::vector<std::vector<std::pair<int, float>>> tmpBoneWeightList(controlPointsCount);	// �{�[���C���f�b�N�X�l�ƃE�F�C�g�l���y�A�ɂ����u�d�b�s�n�q

	// �e�����l����������X�L���������o��
	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
	int clusterCount = skin->GetClusterCount();	// �X�L������N���X�^�i�{�[���j�����擾

	// �N���X�^�[�i�{�[���̎��j�̐������[�v
	for (int i = 0; i < clusterCount; ++i)
	{
		FbxCluster* cluster = skin->GetCluster(i);					// i�Ԗڂ̃N���X�^�i�{�[���j���擾

		// eNormalize�����Ή����Ȃ�
		ExecOrFail(cluster->GetLinkMode() == FbxCluster::eNormalize);	// ���K������Ă��Ȃ��E�F�C�g�l�͑Ή����Ă��Ȃ�

		boneNodeNameList.push_back(cluster->GetLink()->GetName());	// �{�[�������i�[

		int indexCount = cluster->GetControlPointIndicesCount();	// �{�[���ɏ������钸�_�C���f�b�N�X�����擾
		int* indices = cluster->GetControlPointIndices();			// ���_�C���f�b�N�X�z����擾
		double* weights = cluster->GetControlPointWeights();		// �E�F�C�g�l�z����擾

		// �{�[���ɏ������钸�_�C���f�b�N�X�������[�v����
		for (int j = 0; j < indexCount; ++j)
		{
			int controlPointIndex = indices[j];		// ���̃{�[���ɉe�����󂯂�j�Ԗڂ̒��_�C���f�b�N�X�ԍ����擾
			tmpBoneWeightList[controlPointIndex].push_back(std::pair<int, float>(i, static_cast<float>(weights[j])));
			// ���_�C���f�b�N�X�ԍ��̂u�d�b�s�n�q�ʒu�Ɂii�Ԗڂ̃{�[���Ƃ��̃E�G�C�g�l�j
		}

		// �y�[�X�|�[�Y�̋t�s����쐬���Ă���
		FBX::Matrix4x4 invBaseposeMatrix;

		FbxMatrix baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();		// �t�s����v�Z����

		// FbxMatrix->Matrix4x4��
		auto baseposeMatrixPtr = (double*)baseposeMatrix;
		for (int j = 0; j < 16; ++j)
		{
			invBaseposeMatrix.m[j] = static_cast<float>(baseposeMatrixPtr[j]);	// double->float
		}

		invBaseposeMatrixList.push_back(invBaseposeMatrix);			// i�Ԗڂ̃{�[���̏����p���������t�s��
	}

	// �R���g���[���|�C���g�ɑΉ������E�F�C�g���쐬
	std::vector<FBX::BoneWeight> boneWeightListControlPoints;

	for (unsigned int j = 0; j < tmpBoneWeightList.size(); j++) {
		// ���Ԗڂ̒��_�̃E�F�C�g�l���擾
		std::vector<std::pair<int, float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// �E�F�C�g�l�̑傫���Ń\�[�g
		std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
			[](const std::pair<int, float>& weightA, const std::pair<int, float>& weightB) { return weightA.second > weightB.second; }
		);

		// 1���_��4��葽���E�F�C�g������U���Ă���Ȃ�e�������Ȃ����͖̂�������
		while (tmpBoneWeight.size() > 4)
		{
			tmpBoneWeight.pop_back();
		}

		// 4�ɖ����Ȃ��ꍇ�̓_�~�[��}��
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

		// �E�F�C�g�̐��K��
		for (int i = 0; i < 4; ++i)
		{
			weight.boneWeight.v[i] /= total;
		}

		boneWeightListControlPoints.push_back(weight);


	}

	// �C���f�b�N�X�œW�J
	for (unsigned int j = 0; j < indexList.size(); j++) {
		int index = indexList[j];				// j�Ԗڂ̒��_�C���f�b�N�X���擾
		boneWeightList.push_back(boneWeightListControlPoints[index]);	// j�Ԗڂ̒��_�̃{�[���ԍ��ƃE�G�C�g�l���Z�b�g
	}
}

std::vector<FBX::Vector2> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo)
{
	std::vector<FBX::Vector2> uvList;

	int elementCount = mesh->GetElementUVCount();		// ���̂t�u��񂪃Z�b�g����Ă��邩
	if (uvNo + 1 > elementCount)						// ����͂P�ɂ̂ݑΉ�
	{
		return uvList;
	}

	FbxGeometryElementUV* element = mesh->GetElementUV(uvNo);					// UV�Z�b�g���擾
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// �}�b�s���O���[�h���擾
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �t�u�����i�[�����u�d�b�s�n�q�z��̃C���f�b�N�X�z����擾
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// �t�u�l�z����擾

	// eDirct��eIndexDirect�̂ݑΉ�
	ExecOrFail((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// ���_�C���f�b�N�X�����������������e�ʂ��m��
	uvList.reserve(indexList.size());

	// ���_�ɑΉ����Ċi�[����Ă���ꍇ
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// ���_���W�Ń}�b�s���O
		for (unsigned int i = 0; i < indexList.size(); i++)
		{
			int index = indexList[i];			// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾

			// ���t�@�����X���[�h�𔻒�
			int uvIndex;
			if (referenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
				uvIndex = index;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
			}
			else {													// eIndexToDirect�̏ꍇ
				uvIndex = indexArray.GetAt(index);				// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
			}

			FbxVector2 uv = directArray.GetAt(uvIndex);		// uv�l��double�^�Ŏ擾
			uvList.push_back(FBX::Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));		// float�l�Ƃ��Ċi�[
		}
	}
	// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
		int indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
		int polygonCount = mesh->GetPolygonCount();			// ���b�V���̃|���S�������擾
		for (int i = 0; i < polygonCount; ++i)				// �|���S���������[�v
		{
			int polygonSize = mesh->GetPolygonSize(i);		// ���Ԗڂ̃|���S�����_�����擾

			// �|���S���̒��_�������[�v
			for (int j = 0; j < polygonSize; ++j)
			{
				// ���t�@�����X���[�h�̔���H
				int uvIndex;
				if (referenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
					uvIndex = indexByPolygonVertex;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
				}
				else {													// eIndexToDirect�̏ꍇ
					uvIndex = indexArray.GetAt(indexByPolygonVertex);	// �ʂ̍\�����C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
				}
				FbxVector2 uv = directArray.GetAt(uvIndex);

				uvList.push_back(FBX::Vector2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));	// �����������l�Ƃ��Ċi�[

				++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
			}
		}
	}
	else
	{
		// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
		ExecOrFail(false);
	}

	return uvList;
}

std::vector<FBX::Vector3> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList) {

	int elementCount = mesh->GetElementNormalCount();			// ���̖@����񂪃Z�b�g����Ă��邩

	ExecOrFail(elementCount == 1);					// �P�̖@�����ɂ����Ή����Ȃ�

	FbxGeometryElementNormal* element = mesh->GetElementNormal(0);	// �O�Ԗڂ̖@���Z�b�g���擾
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

	// eDirect���@eIndexDirect�̂ݑΉ�
	ExecOrFail((referrenceMode == FbxGeometryElement::eDirect) || (referrenceMode == FbxGeometryElement::eIndexToDirect));

	// �@�����u�d�b�s�n�q
	std::vector<FBX::Vector3> normalList;
	normalList.reserve(indexList.size());		// ���_�C���f�b�N�X�����m��

	// ���_���W�Ń}�b�s���O����Ă���ꍇ
	if (mappingmode == FbxGeometryElement::eByControlPoint) {
		for (unsigned int i = 0; i < indexList.size(); i++) {
			int index = indexList[i];		// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
			int normalIndex;				// �@�����̃C���f�b�N�X
			// ���t�@�����X���[�h�𔻒�
			if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
				normalIndex = index;								// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
			}
			else {													// eIndexToDirect�̏ꍇ
				normalIndex = indexArray.GetAt(index);				// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾
			}
			FbxVector4 normal = directArray.GetAt(normalIndex);		// �@�������擾
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.push_back(FBX::Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));			// �������W��
				}
				else {
					normalList.push_back(FBX::Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));		// �������W��
				}
			}
		}
	}
	else if (mappingmode == FbxGeometryElement::eByPolygonVertex) {
		// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// �|���S�������擾
		for (int i = 0; i < polygonCount; ++i) {
			int polygonSize = mesh->GetPolygonSize(i);				// ���_�����擾
			int normalIndex;
			for (int j = 0; j < polygonSize; ++j) {
				// ���t�@�����X���[�h�𔻒�
				if (referrenceMode == FbxGeometryElement::eDirect) {		// eDirect�̏ꍇ
					normalIndex = indexByPolygonVertex;
				}
				else {
					normalIndex = indexArray.GetAt(indexByPolygonVertex);	// eIndexToDirect�̏ꍇ
				}
				FbxVector4 normal = directArray.GetAt(normalIndex);		// �@�������擾
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0) {
						normalList.push_back(FBX::Vector3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));		// �������W��
					}
					else {
						normalList.push_back(FBX::Vector3(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3])));	// �������W��
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
	//�@���_���W���擾
	std::vector<FBX::Vector3> positionList;					//	���_���W�i�[�u�d�b�s�n�q 
	positionList.reserve(indexList.size());				//  

	for (unsigned int i = 0; i < indexList.size(); i++) {
		int index = indexList[i];						// i�Ԗڂ̒��_���i�ʂ̍\�����񏇂ɕ���ł���j���擾
		FbxVector4 Position = mesh->GetControlPointAt(index);		// index�Ԗڂ̒��_���W���擾
		if (Position[3] != 0.0) {
			if (Position[3] == 1.0) {
				positionList.push_back(FBX::Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));			// �������W��
			}
			else {
				positionList.push_back(FBX::Vector3(static_cast<float>(Position[0] / Position[3]), static_cast<float>(Position[1] / Position[3]), static_cast<float>(Position[2] / Position[3])));		// �������W��
			}
		}
		else {
			positionList.push_back(FBX::Vector3(static_cast<float>(Position[0]), static_cast<float>(Position[1]), static_cast<float>(Position[2])));				// �v�𖳎����Ċi�[
		}
	}
	return positionList;
}

std::vector<int> GetIndexList(FbxMesh* mesh) {

	int polygonCount = mesh->GetPolygonCount();			// �O�p�`�̐����擾
	std::vector<int> indexList;							// ���_�C���f�b�N�X�i�ʂ̍\�����j
	indexList.reserve(polygonCount * 3);				// 3�p�`�̐��~�R

	// �ʂ̍\�������擾����
	for (int i = 0; i < polygonCount; i++) {
		indexList.push_back(mesh->GetPolygonVertex(i, 0));		// i�Ԗڂ̎O�p�`�̂O�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(mesh->GetPolygonVertex(i, 1));		// i�Ԗڂ̎O�p�`�̂P�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(mesh->GetPolygonVertex(i, 2));		// i�Ԗڂ̎O�p�`�̂Q�Ԗڂ̒��_�C���f�b�N�X���擾
	}
	return indexList;
}

FBX::Mesh ParseMesh(FbxMesh* mesh) {

	ExecOrFail(mesh != nullptr);

	FbxNode* fbxnode = mesh->GetNode();					// �e�a�w�m�[�h���擾

	// �}�e���A����1�̂��̂����Ή����Ȃ�
	ExecOrFail(fbxnode->GetMaterialCount() == 1);

	FBX::Mesh modelMesh;								// �ŏI�I�ɕۑ�����郁�b�V�����i���̊֐��̏o�͒l�j

	// �����X�L���̐����擾
	modelMesh.skincount = mesh->GetDeformerCount(FbxDeformer::eSkin);		// �����X�L�����̐����Z�b�g
	modelMesh.nodeName = fbxnode->GetName();								// �e�a�w�m�[�h�����Z�b�g
	modelMesh.materialName = fbxnode->GetMaterial(0)->GetName();			// �}�e���A�������Z�b�g

//	printf("mesh:%s ,%s \n", modelMesh.nodeName.c_str(), modelMesh.materialName.c_str());

	// �x�[�X�|�[�Y�̋t�s����쐬���Ă���(���b�V����)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	double* baseposeMatrixPtr = (double*)baseposeMatrix;

	for (int i = 0; i < 16; ++i) {
		modelMesh.invMeshBaseposeMatrix.m[i] = static_cast<float>(baseposeMatrixPtr[i]);
	}

	// �C���f�b�N�X�u�d�b�s�n�q�擾
	std::vector<int> indexList = GetIndexList(mesh);

	// ���_�f�[�^�擾
	std::vector<FBX::Vector3> positionList = GetPositionList(mesh, indexList);		// ���_���W���擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<FBX::Vector3> normalList = GetNormalList(mesh, indexList);			// �@���x�N�g�����擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<FBX::Vector2> uv0List = GetUVList(mesh, indexList, 0);				// UV���W���擾�i�ʂ̍\�����ɍ��킹��j

	// �E�G�C�g�l���擾����
	std::vector<FBX::BoneWeight> boneWeightList;
	GetWeight(mesh,											// ���b�V���i���́j
		indexList,										// �ʂ̍\�����i���́j
		boneWeightList,									// �{�[����񃊃X�g�i�o�͒l�j�i�{�[�����ւ̃C���f�b�N�X�l�ƃE�F�C�g�l�j
		modelMesh.boneNodeNameList,						// ���b�V�����̃{�[�������u�d�b�s�n�q�ɕۑ��i�o�́j
		modelMesh.invBoneBaseposeMatrixList);			// �{�[���I�t�Z�b�g�s����u�d�b�s�n�q�ɕۑ��i�j

// �O�̂��߃T�C�Y�`�F�b�N
	ExecOrFail(indexList.size() == positionList.size());
	ExecOrFail(indexList.size() == normalList.size());
	ExecOrFail(indexList.size() == uv0List.size());
	ExecOrFail((indexList.size() == boneWeightList.size()) || (boneWeightList.size() == 0));

	// �e���|�����̒��_���i�l���������u�����������j�����
	std::vector<FBX::Vertex> tmpmodelVertexList;
	tmpmodelVertexList.reserve(indexList.size());		// ���_�C���f�b�N�X�T�C�Y���e�ʂ��m��

	// ���_�C���f�b�N�X�����i�ʂ̍\����񏇁j���[�v����
	for (unsigned int i = 0; i < indexList.size(); ++i)
	{
		FBX::Vertex vertex;
		vertex.position = positionList[i];			// ���_���
		vertex.normal = normalList[i];				// �@�����

		if (uv0List.size() == 0) {
			vertex.uv0 = FBX::Vector2(0.0f, 0.0f);
		}
		else {
			vertex.uv0 = uv0List[i];
		}

		// �{�[���E�F�C�g�l�����݂��邩�H
		if (boneWeightList.size() > 0)
		{
			// �{�[���̃C���f�b�N�X�ԍ����Z�b�g
			for (int j = 0; j < 4; ++j)
			{
				vertex.boneIndex[j] = boneWeightList[i].boneIndex[j];
			}
			// �E�F�C�g�l���Z�b�g
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

	// �d�����_�������ăC���f�b�N�X�쐬
	modelMesh.vertexList.reserve(tmpmodelVertexList.size());		// ���_��񐔕��G���A���m�ۂ���

	modelMesh.indexList.reserve(indexList.size());		// �ʂ̍\����񐔕��G���A���m�ۂ���

	// �����ŏd�����Ă��钸�_�f�[�^�������A���_�f�[�^���X�g�ƃC���f�b�N�X���X�g����蒼���Ă���
	// �d���������_���܂񂾒��_�������[�v���Ă���(�ʂ̍\���������ɔ����o�������_���)
	for (unsigned int i = 0; i < tmpmodelVertexList.size(); i++)
	{
		FBX::Vertex & vertex = tmpmodelVertexList[i];
		// �d�����Ă��邩�H
		// modelMesh.vertexList�́A�ŏ���ł��񂾂�o�^����Ă����i�d�����Ă��Ȃ����_���Ƃ��āj
		auto it = std::find(modelMesh.vertexList.begin(), modelMesh.vertexList.end(), vertex);
		if (it == modelMesh.vertexList.end())
		{
			// �d�����Ă��Ȃ�
			modelMesh.indexList.push_back(modelMesh.vertexList.size());	// ���_�C���f�b�N�X���i�[
			modelMesh.vertexList.push_back(vertex);					// ���_�����i�[
		}
		else
		{
			// �d�����Ă���
			auto index = std::distance(modelMesh.vertexList.begin(), it);	// �擪���猻�C�e���[�^�i�d�����_�����擪�f�[�^���w�������Ă���j�܂ł̃C���f�b�N�X�ԍ����擾
			modelMesh.indexList.push_back(index);// �C���f�b�N�X�ԍ��i�d�����_�����擪�f�[�^���w�������Ă���j���C���f�b�N�X���X�g�ɃZ�b�g
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

	// �}�e���A�����擾
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

	// �e�N�X�`�����擾
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

	// FbxManager����
	manager = FbxManager::Create();
	if (!manager) exit(1);

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	// Scene����
	scene = FbxScene::Create(manager, "originalScene");
	if (!scene) exit(1);

	// Importer����
	importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize(modelFile, -1, manager->GetIOSettings())) exit(1);

	// FBX���p�[�X����Scene�ɓǂݍ���
	if (!importer->Import(scene)) exit(1);

	// �C���|�[�^�[���
	importer->Destroy();

	// ���炩���߃|���S����S�ĂR�p�`�����Ă���
	FbxGeometryConverter geometryConverter(manager);
	geometryConverter.Triangulate(scene, true);

	// �V�[���Ɋ܂܂�郁�b�V���̉��
	auto meshCount = scene->GetMemberCount<FbxMesh>();
	printf("���b�V���̐� %d \n", meshCount);

	int nodeCount = scene->GetNodeCount();
	printf("�m�[�h�̐� %d \n", nodeCount);

	// �m�[�h������m�[�hID���擾�ł���悤�Ɏ����ɓo�^
	for (auto i = 0; i < nodeCount; i++) {
		FbxNode* fbxNode = scene->GetNode(i);
		nodeIdDictionary.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	// �m�[�h�̒��g���m�F
	for (std::map<std::string, int>::iterator it = nodeIdDictionary.begin(); it != nodeIdDictionary.end(); ++it) {
		//		std::cout << "key = " << (*it).first << ",value = " << (*it).second << std::endl;
	}

	// �V�[���Ɋ܂܂��}�e���A���̉��
	static int materialCount = scene->GetMaterialCount();
	printf("�}�e���A���̐� %d \n", materialCount);

	materialList.reserve(materialCount);
	for (int i = 0; i < materialCount; ++i) {
		FbxSurfaceMaterial* fbxMaterial = scene->GetMaterial(i);


		Material modelMaterial = ParseMaterial(fbxMaterial);
		materialList.push_back(modelMaterial);
		materialIdDictionary.insert(std::pair<std::string, int>(modelMaterial.materialName, i));
	}

	// �V�[���Ɋ܂܂�郁�b�V���̉��
	meshCount = scene->GetMemberCount<FbxMesh>();
	meshList.reserve(meshCount);
	// ts	printf("meshCount: %d\n", meshCount);

	for (int i = 0; i < meshCount; ++i) {
		FbxMesh* fbxMesh = scene->GetMember<FbxMesh>(i);
		meshList.push_back(ParseMesh(fbxMesh));
	}

	// �A�j���[�V�����ǂݍ���
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
	Mesh & modelMesh = meshList[meshId];				// �����ŗ������@t.s 2014/08/09(�C������)

	if (fbxSceneAnimation == nullptr) {
		for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); ++i) {
			out_matrixList[i] = matrix4x4Identity;
		}
		return;
	}


	if (modelMesh.boneNodeNameList.empty())				// �{�[�������X�g����Ȃ�
	{
		out_matrixList[0] = matrix4x4Identity;
		return;
	}

	ExecOrFail(modelMesh.boneNodeNameList.size() <= matrixCount);	// �{�[�������z�񐔂�葽���ꍇ�����I��

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);	// �w��t���[�����ɉ������o�ߎ��Ԃ��Z�b�g

	// ���̃��b�V���Ɋ܂܂�Ă���{�[���������[�v
	for (unsigned int i = 0; i < modelMesh.boneNodeNameList.size(); ++i)
	{
		std::string& boneNodeName = modelMesh.boneNodeNameList[i];			// �{�[���m�[�h���擾
		int boneNodeId = nodeIdDictionaryAnimation.at(boneNodeName);		// �{�[���m�[�h������{�[���m�[�h�h�c�擾
		FbxNode* boneNode = fbxSceneAnimation->GetNode(boneNodeId);			// �A�j���[�V�����m�[�h�擾

		FbxMatrix boneMatrix = boneNode->EvaluateGlobalTransform(time);		// �o�ߎ��Ԃɉ������p��������킷�{�[���s����擾
		Matrix4x4& out_matrix = out_matrixList[i];

		// FbxMatrix�idouble�̔z��j����Matrix4x4(float�̔z��)�֕ϊ�
		double* boneMatrixPtr = (double*)boneMatrix;
		for (int j = 0; j < 16; ++j)
		{
			out_matrix.m[j] = static_cast<float>(boneMatrixPtr[j]);			// double->float
		}

		MulMatrixMatrix(modelMesh.invBoneBaseposeMatrixList[i], out_matrix, out_matrix);		// �{�[���I�t�Z�b�g�s�񁖃{�[���s��
	}
}

void FBX::getMeshMatrix(INT64 frame, int meshId, Matrix4x4 & out_matrix)
{
	Mesh & modelMesh = meshList[meshId];

	if (fbxSceneAnimation == nullptr) {
		out_matrix = matrix4x4Identity;
		return;
	}

	std::map<std::string, int>::iterator it = nodeIdDictionaryAnimation.find(modelMesh.nodeName);		// ���b�V���̃m�[�h������m�[�h�h�c���擾
	// ������Ȃ������ꍇ
	if (it == nodeIdDictionaryAnimation.end())
	{
		out_matrix = matrix4x4Identity;
		return;
	}

	int meshNodeId = it->second;											// �m�[�h�h�c�擾
	FbxNode* meshNode = fbxSceneAnimation->GetNode(meshNodeId);				// FBX�m�[�h���擾

	FbxTime time;
	time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);		// �w��t���[���ɉ������o�ߎ��Ԃ��Z�b�g

	FbxMatrix meshMatrix = meshNode->EvaluateGlobalTransform(time);			// �o�ߎ��Ԃɉ������p��������킷�s����擾

	// FbxMatrix�idouble�̔z��j����Matrix4x4(float�̔z��)�֕ϊ�
	const auto meshMatrixPtr = static_cast<double*>(meshMatrix);
	for (int i = 0; i < 16; ++i)
	{
		out_matrix.m[i] = static_cast<float>(meshMatrixPtr[i]);				// double->float
	}

	MulMatrixMatrix(modelMesh.invMeshBaseposeMatrix, out_matrix, out_matrix);		// �I�t�Z�b�g�s�񁖃��b�V���p���s��
}

void FBX::loadAnimation(const char * animationfilename)
{
	// ���łɃA�j���[�V������ǂݍ���ł�����
	if (fbxSceneAnimation) {
		fbxSceneAnimation->Destroy();
		fbxSceneAnimation = nullptr;
		nodeIdDictionaryAnimation.clear();
	}

	// �A�j���[�V�������擾
	// Importer����
	importer = FbxImporter::Create(manager, "");

	if (!importer->Initialize(animationfilename, -1, manager->GetIOSettings())) exit(1);

	// animationScene����
	fbxSceneAnimation = FbxScene::Create(manager, "animationScene");
	if (!fbxSceneAnimation) exit(1);

	// FBX���p�[�X����fbxSceneAnimation�ɓǂݍ���
	if (!importer->Import(fbxSceneAnimation)) exit(1);

	// �A�j���[�V�������擾
	ExecOrFail(importer->GetAnimStackCount() == 1);								// �P�A�j���[�V���������ɑΉ�

	FbxTakeInfo* takeInfo = importer->GetTakeInfo(0);				// �O�Ԗڂ̂s�`�j�d���擾

	FbxTime importOffset = takeInfo->mImportOffset;					// �I�t�Z�b�g����
	FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();		// �J�n�������擾
	FbxTime stopTime = takeInfo->mLocalTimeSpan.GetStop();			// �I���������擾

	// �A�j���[�V�����J�n���Ԏ��ԁ^�P�t���[���̎��ԂŊJ�n�t���[�������߂Ă���
	animationStartFrame = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	// �A�j���[�V�����I�����ԁ^�P�t���[���̎��Ԃ̎��ԂŏI���t���[�������߂Ă���
	animationEndFrame = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	importer->Destroy();

	// �m�[�h������m�[�hID���擾�ł���悤�Ɏ����ɓo�^
	int nodeCount = fbxSceneAnimation->GetNodeCount();
	printf("animationNodeCount: %d\n", nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		auto fbxNode = fbxSceneAnimation->GetNode(i);
		nodeIdDictionaryAnimation.insert(std::pair<std::string, int>(fbxNode->GetName(), i));
	}

	// �f�o�b�O�p
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
	// �}�e���A�����ŃC���f�b�N�X���T�[�`
	return materialList[(*materialIdDictionary.find(materialname)).second];
}

bool FBX::isAnimationEnd(INT64 frame)
{
	return fbxSceneAnimation ? frame > animationEndFrame : false;
}
