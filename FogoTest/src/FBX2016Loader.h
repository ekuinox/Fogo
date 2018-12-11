#pragma once

// https://github.com/akinobufujii/D3D12OnFBX2016Sample

#include <DirectXMath.h>
#include <vector>

struct VertexData {
	DirectX::XMFLOAT3 pos;		// 座標
	DirectX::XMFLOAT4 normal;	// 法線
	DirectX::XMFLOAT2 uv;		// UV座標
};

using VertexContainer = std::vector<VertexData>;

void loadFBXConvertToVertexData(const char* filename, VertexContainer & outVertexData);