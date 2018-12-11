#pragma once

// https://github.com/akinobufujii/D3D12OnFBX2016Sample

#include <DirectXMath.h>
#include <vector>

struct VertexData {
	DirectX::XMFLOAT3 pos;		// ���W
	DirectX::XMFLOAT4 normal;	// �@��
	DirectX::XMFLOAT2 uv;		// UV���W
};

using VertexContainer = std::vector<VertexData>;

void loadFBXConvertToVertexData(const char* filename, VertexContainer & outVertexData);