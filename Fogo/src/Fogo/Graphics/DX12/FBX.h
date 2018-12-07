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
				float m[16];				// �P�����z��
				float m2[4][4];			// �Q�����z��
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

		// �{�[�����
		struct BoneWeight {
			unsigned int boneIndex[4];				// �{�[���C���f�b�N�X�ԍ�
			Vector4 boneWeight;					// �{�[���E�G�C�g�l
		};

		// ���_���\����
		struct Vertex {
			Vector3 position;						// ���W
			Vector3 normal;							// �@�����
			Vector2 uv0;							// �e�N�X�`�����W
			unsigned int	boneIndex[4];			// �g�p����{�[���s��̃C���f�b�N�X
			Vector4			boneWeight;				// �E�F�C�g�l
			bool operator == (const Vertex & v) const {		//�@�������Z�q�̃I�[�o�[���[�h 
				return std::memcmp(this, &v, sizeof(Vertex)) == 0;
			}
			Vertex() : boneIndex{} {
				boneWeight = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				position = Vector3(0.0f, 0.0f, 0.0f);
				normal = Vector3(0.0f, 0.0f, 0.0f);
				uv0 = Vector2(0.0f, 0.0f);
			}
		};

		// ���b�V�����\����	
		struct Mesh {
			int							skincount;			// �����X�L���̐�
			std::string					nodeName;			// FBX�m�[�h��
			std::string					materialName;		// �g�p�}�e���A����

			std::vector<Vertex>	vertexList;			// ���_���i�[�u�d�b�s�n�q�R���e�i
			std::vector<unsigned int>	indexList;			// �ʂ̍\�����u�d�b�s�n�q�R���e�i
			Matrix4x4					invMeshBaseposeMatrix;	// ���b�V���̏����p��
			std::vector<std::string>	boneNodeNameList;		// �{�[���m�[�h���i�[�u�d�b�s�n�q�R���e�i
			std::vector<Matrix4x4> 		invBoneBaseposeMatrixList;	// �{�[���̏����p���̋t�s��i�[�u�d�b�s�n�q�R���e�i
		};

		struct Material {
			std::string materialName;
			std::string diffuseTextureName;
			std::string normalTextureName;
			std::string specularTextureName;
			std::string falloffTextureName;
			std::string reflectionMapTextureName;

			Vector4		Diffuse;		//�f�B�t���[�Y
			Vector4		Speqular;		//�X�y�L����
			Vector4		Ambient;		//�A���r�G���g
			float		Power;			//���˂̋���
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

		std::vector<Mesh>	meshList;					// ModelMesh���i�[����������������
		std::map<std::string, int> nodeIdDictionary;		// ���b�V��������meshList�ԍ����擾���邽�߂̂l�`�o
		std::vector<Material> materialList;			// ModelMaterial���i�[����������������
		std::map<std::string, int> materialIdDictionary;	// �}�e���A��������materialList�ԍ����擾���邽�߂̂l�`�o

		// FBX�p�[�X�p
		FbxManager*		manager;		// SDK�̃������Ǘ�
		FbxScene*		scene;			// �m�[�h�⃁�b�V���A�}�e���A�������܂񂾃V�[�����
		FbxImporter*	importer;		// FBX����͂���FbxScene�ɓW�J

		// �A�j���[�V�����p
		FbxScene* fbxSceneAnimation;	// �A�j���[�V���������܂񂾃V�[�����
		std::map<std::string, int> nodeIdDictionaryAnimation;		// �{�[��������m�[�h�ԍ����擾���邽�߂̂l�`�o
		INT64 animationStartFrame = 0;		// �J�n�t���[���ԍ�
		INT64 animationEndFrame = 0;		// �I���t���[���ԍ�
	};
}
