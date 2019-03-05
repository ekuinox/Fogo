#include "FBXParser.h"
#include "../../Utility/HelperFunctions.h"

using namespace Fogo;
using namespace DirectX;

std::pair<FbxDouble3, std::vector<std::string>> GetMaterialProperty(const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName) {
	FbxDouble3 result(0, 0, 0);
	std::vector<std::string> texturePaths{};

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


std::vector<int> FBXParser::GetIndexes(FbxMesh* mesh) {
	const auto count = mesh->GetPolygonCount();			// �O�p�`�̐����擾
	std::vector<int> indexList;							// ���_�C���f�b�N�X�i�ʂ̍\�����j
	indexList.reserve(count * 3);				// 3�p�`�̐��~�R

	// �ʂ̍\�������擾����
	for (unsigned int i = 0; i < count; i++) {
		indexList.push_back(mesh->GetPolygonVertex(i, 0));		// i�Ԗڂ̎O�p�`�̂O�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(mesh->GetPolygonVertex(i, 1));		// i�Ԗڂ̎O�p�`�̂P�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back(mesh->GetPolygonVertex(i, 2));		// i�Ԗڂ̎O�p�`�̂Q�Ԗڂ̒��_�C���f�b�N�X���擾
	}
	return indexList;
}

std::vector<XMFLOAT3> FBXParser::GetPositions(FbxMesh* mesh, const std::vector<int> & indexes) {
	//�@���_���W���擾
	std::vector<XMFLOAT3> positionList {};					//	���_���W�i�[�u�d�b�s�n�q 
	positionList.reserve(indexes.size());				//  

	for (unsigned int i = 0; i < indexes.size(); ++i) {
		const auto position = mesh->GetControlPointAt(indexes[i]); // index�Ԗڂ̒��_���W���擾
		if (position[3] != 0.0) {
			if (position[3] == 1.0) {
				positionList.emplace_back(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]));			// �������W��
			}
			else {
				positionList.emplace_back(static_cast<float>(position[0] / position[3]), static_cast<float>(position[1] / position[3]), static_cast<float>(position[2] / position[3]));		// �������W��
			}
		}
		else {
			positionList.emplace_back(static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2]));				// �v�𖳎����Ċi�[
		}
	}
	return positionList;
}

std::vector<XMFLOAT3> FBXParser::GetNormals(FbxMesh * mesh, const std::vector<int> & indexes) {
	const auto elementCount = mesh->GetElementNormalCount();			// ���̖@����񂪃Z�b�g����Ă��邩

	assert(elementCount == 1);					// �P�̖@�����ɂ����Ή����Ȃ�

	const auto element = mesh->GetElementNormal(0);	// �O�Ԗڂ̖@���Z�b�g���擾
	const auto mappingMode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
	const auto referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
	const auto & indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
	const auto & directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

	// eDirect���@eIndexDirect�̂ݑΉ�
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// �@�����u�d�b�s�n�q
	std::vector<XMFLOAT3> normalList;
	normalList.reserve(indexes.size());		// ���_�C���f�b�N�X�����m��

	// ���_���W�Ń}�b�s���O����Ă���ꍇ
	if (mappingMode == FbxGeometryElement::eByControlPoint) {
		for (const auto & index : indexes) {
			const auto normal = directArray.GetAt(
				referenceMode == FbxGeometryElement::eDirect ? index : indexArray.GetAt(index)
			); 		// �@�������擾
			if (normal[3] != 0.0f) {
				if (normal[3] == 1.0f) {
					normalList.emplace_back(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]));			// �������W��
				}
				else {
					normalList.emplace_back(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3]));		// �������W��
				}
			}
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
		// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
		auto indexByPolygonVertex = 0;
		const auto polygonCount = mesh->GetPolygonCount();					// �|���S�������擾
		for (unsigned int i = 0; i < polygonCount; ++i) {
			const auto polygonSize = mesh->GetPolygonSize(i);				// ���_�����擾
			for (unsigned int j = 0; j < polygonSize; ++j) {
				// ���t�@�����X���[�h�𔻒�
				auto normal = directArray.GetAt(
					referenceMode == FbxGeometryElement::eDirect ? indexByPolygonVertex : indexArray.GetAt(indexByPolygonVertex)
				); // �@�������擾
				if (normal[3] != 0.0f) {
					if (normal[3] == 1.0) {
						normalList.emplace_back(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2]));		// �������W��
					}
					else {
						normalList.emplace_back(static_cast<float>(normal[0] / normal[3]), static_cast<float>(normal[1] / normal[3]), static_cast<float>(normal[2] / normal[3]));	// �������W��
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

	const auto elementCount = mesh->GetElementUVCount();		// ���̂t�u��񂪃Z�b�g����Ă��邩
	if (uvNo + 1 > elementCount)						// ����͂P�ɂ̂ݑΉ�
	{
		return uvList;
	}

	const auto element = mesh->GetElementUV(uvNo);					// UV�Z�b�g���擾
	const auto mappingMode = element->GetMappingMode();		// �}�b�s���O���[�h���擾
	const auto referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	
	const auto & indexArray = element->GetIndexArray();	// �t�u�����i�[�����u�d�b�s�n�q�z��̃C���f�b�N�X�z����擾
	const auto & directArray = element->GetDirectArray();	// �t�u�l�z����擾

	// eDirct��eIndexDirect�̂ݑΉ�
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	// ���_�C���f�b�N�X�����������������e�ʂ��m��
	uvList.reserve(indexes.size());

	// ���_�ɑΉ����Ċi�[����Ă���ꍇ
	if (mappingMode == FbxGeometryElement::eByControlPoint)	{
		// ���_���W�Ń}�b�s���O
		for (const auto & index : indexes) {
			// ���t�@�����X���[�h�𔻒�
			const auto uv = directArray.GetAt(
				referenceMode == FbxGeometryElement::eDirect ? index : indexArray.GetAt(index)
			);
			uvList.emplace_back(static_cast<float>(uv[0]), static_cast<float>(uv[1]));		// float�l�Ƃ��Ċi�[
		}
	}
	// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
		// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
		auto indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
		const auto polygonCount = mesh->GetPolygonCount();			// ���b�V���̃|���S�������擾
		for (unsigned int i = 0; i < polygonCount; ++i)				// �|���S���������[�v
		{
			const auto polygonSize = mesh->GetPolygonSize(i);		// ���Ԗڂ̃|���S�����_�����擾

			// �|���S���̒��_�������[�v
			for (unsigned int j = 0; j < polygonSize; ++j)
			{
				// ���t�@�����X���[�h�̔���H
				const auto uv = directArray.GetAt(
					referenceMode == FbxGeometryElement::eDirect ? indexByPolygonVertex : indexArray.GetAt(indexByPolygonVertex)
				);
				uvList.emplace_back(static_cast<float>(uv[0]), static_cast<float>(uv[1]));	// �����������l�Ƃ��Ċi�[

				++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
			}
		}
	}
	else {
		// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
		assert(false);
	}

	return uvList;
}


FBXParser::Mesh FBXParser::Parse(FbxMesh* mesh) {
	assert(mesh != nullptr);

	const auto node = mesh->GetNode();					// �e�a�w�m�[�h���擾

	// �}�e���A����1�̂��̂����Ή����Ȃ�
	assert(node->GetMaterialCount() == 1);

	Mesh modelMesh;								// �ŏI�I�ɕۑ�����郁�b�V�����i���̊֐��̏o�͒l�j

	// �����X�L���̐����擾
	modelMesh.skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);		// �����X�L�����̐����Z�b�g
	modelMesh.nodeName = node->GetName();								// �e�a�w�m�[�h�����Z�b�g
	modelMesh.materialName = node->GetMaterial(0)->GetName();			// �}�e���A�������Z�b�g

	// �C���f�b�N�X�u�d�b�s�n�q�擾
	const auto & indexes = GetIndexes(mesh);

	// ���_�f�[�^�擾
	const auto & positions = GetPositions(mesh, indexes);		// ���_���W���擾�i�ʂ̍\�����ɍ��킹��j
	const auto & normals = GetNormals(mesh, indexes);			// �@���x�N�g�����擾�i�ʂ̍\�����ɍ��킹��j
	const auto & uvs = GetUVs(mesh, indexes);					// UV���W���擾�i�ʂ̍\�����ɍ��킹��j

	// �O�̂��߃T�C�Y�`�F�b�N

	/*
	assert(indexes.size() == positions.size());
	assert(indexes.size() == normals.size());
	assert(indexes.size() == uvs.size());
	*/

	// �e���|�����̒��_���i�l���������u�����������j�����
	std::vector<Vertex> tempVertexes;
	tempVertexes.reserve(indexes.size());		// ���_�C���f�b�N�X�T�C�Y���e�ʂ��m��

	// ���_�C���f�b�N�X�����i�ʂ̍\����񏇁j���[�v����
	for (unsigned int i = 0; i < indexes.size(); ++i) {
		tempVertexes.emplace_back(Vertex {
			positions[i],
			normals.empty() ? XMFLOAT3(0.0f, 0.0f, 0.0f) : normals[i],
			uvs.empty() ? XMFLOAT2(0.0f, 0.0f) : uvs[i] }
		);
	}

	// �d�����_�������ăC���f�b�N�X�쐬
	modelMesh.vertexes.reserve(tempVertexes.size());		// ���_��񐔕��G���A���m�ۂ���

	modelMesh.indexes.reserve(indexes.size());		// �ʂ̍\����񐔕��G���A���m�ۂ���

	// �����ŏd�����Ă��钸�_�f�[�^�������A���_�f�[�^���X�g�ƃC���f�b�N�X���X�g����蒼���Ă���
	// �d���������_���܂񂾒��_�������[�v���Ă���(�ʂ̍\���������ɔ����o�������_���)
	for (const auto & vertex : tempVertexes) {
		// �d�����Ă��邩�H
		// modelMesh.vertexList�́A�ŏ���ł��񂾂�o�^����Ă����i�d�����Ă��Ȃ����_���Ƃ��āj
		auto it = std::find(modelMesh.vertexes.begin(), modelMesh.vertexes.end(), vertex);
		if (it == modelMesh.vertexes.end())	{
			// �d�����Ă��Ȃ�
			modelMesh.indexes.push_back(modelMesh.vertexes.size());	// ���_�C���f�b�N�X���i�[
			modelMesh.vertexes.push_back(vertex);					// ���_�����i�[
		}
		else {
			// �d�����Ă���
			modelMesh.indexes.push_back(std::distance(modelMesh.vertexes.begin(), it));// �C���f�b�N�X�ԍ��i�d�����_�����擪�f�[�^���w�������Ă���j���C���f�b�N�X���X�g�ɃZ�b�g
		}
	}

	return modelMesh;
}

FBXParser::Material FBXParser::Parse(FbxSurfaceMaterial* material) {
	auto modelMaterial = Material {};
	modelMaterial.name = material->GetName();

	// �}�e���A�����擾
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

	// �e�N�X�`�����擾
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

				std::string texName = fbxProperty.GetSrcObject<FbxFileTexture>(j)->GetFileName();
				texName = texName.substr(texName.find_last_of('/') + 1);

				if (src == "Maya|DiffuseTexture") {
					modelMaterial.texturePath = texName.substr(texName.find_last_of('/') + 1);
				}
			}
		}
	} else {
		const auto & emissiveProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
		const auto & ambientProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
		const auto & diffuseProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
		const auto & specularProperty = GetMaterialProperty(material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

		modelMaterial.emissive = DirectX::XMFLOAT3(emissiveProperty.first.mData[0], emissiveProperty.first.mData[1], emissiveProperty.first.mData[2]);
		modelMaterial.ambient = DirectX::XMFLOAT3(ambientProperty.first.mData[0], ambientProperty.first.mData[1], ambientProperty.first.mData[2]);
		modelMaterial.diffuse = DirectX::XMFLOAT3(diffuseProperty.first.mData[0], diffuseProperty.first.mData[1], diffuseProperty.first.mData[2]);
		modelMaterial.specular = DirectX::XMFLOAT3(specularProperty.first.mData[0], specularProperty.first.mData[1], specularProperty.first.mData[2]);

		for (const auto & texturePath : emissiveProperty.second) modelMaterial.texturePath = texturePath;
		for (const auto & texturePath : ambientProperty.second) modelMaterial.texturePath = texturePath;
		for (const auto & texturePath : diffuseProperty.second) modelMaterial.texturePath = texturePath;
		for (const auto & texturePath : specularProperty.second) modelMaterial.texturePath = texturePath;
	}

	return modelMaterial;
}

std::vector<FBXParser::Mesh> FBXParser::Parse(FbxScene * scene) {
	auto & meshes = GetMeshes(scene);
	const auto & materials = GetMaterials(scene);
	for (auto && mesh : meshes) {
		mesh.material = materials.at(mesh.materialName);
	}
	return meshes;
}

std::unordered_map<std::string, FBXParser::Material> FBXParser::GetMaterials(FbxScene * scene) {
	std::unordered_map<std::string, Material> materials {};
	const auto count = scene->GetMaterialCount();
	for (unsigned int i = 0; i < count; ++i) {
		const auto & material = Parse(scene->GetMaterial(i));
		materials[material.name] = material;
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

FbxScene * FBXParser::Triangulate(FbxScene * scene, FbxManager * manager, bool isLegacy) {
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);
	return scene;
}

FBXParser::FBXParser() {
	__manager = FbxManager::Create();
	__scene = FbxScene::Create(__manager, "");
	__io_settings = FbxIOSettings::Create(__manager, IOSROOT);
	__manager->SetIOSettings(__io_settings);
}

FBXParser & FBXParser::import(const char * file) {
	const auto importer = FbxImporter::Create(__manager, "");

	ExecOrFail(importer->Initialize(file, -1, __manager->GetIOSettings()));
	ExecOrFail(importer->Import(__scene));

	importer->Destroy();

	return * this;
}

FBXParser & FBXParser::parse() {
	__meshes = Parse(__scene);

	return * this;
}

FBXParser & FBXParser::triangulate(bool isLegacy) {
	Triangulate(__scene, __manager, isLegacy);
	return * this;
}

FBXParser & FBXParser::loadTextures(LPCWSTR directory) {
	for (auto & mesh : __meshes) {
		if (!mesh.material.texturePath.empty()) {
			const auto textureFile = directory + std::wstring(mesh.material.texturePath.begin(), mesh.material.texturePath.end());
			const auto ext = textureFile.substr(textureFile.find_last_of('.') + 1);
			mesh.material.texture = std::make_shared<Texture>(
				textureFile.c_str(),
				ext == L"tga" || ext == L"TGA" ? Texture::Type::TGA : Texture::Type::ANY
			);
		}
	}
	return * this;
}

std::vector<FBXParser::Mesh> FBXParser::getMeshes() {
	return __meshes;
}

FBXParser::~FBXParser() {
	__io_settings->Destroy();
	__scene->Destroy();
	__manager->Destroy();
}
