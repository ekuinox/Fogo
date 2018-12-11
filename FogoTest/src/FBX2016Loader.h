#pragma once

// https://github.com/akinobufujii/D3D12OnFBX2016Sample

#include <DirectXMath.h>
#include <vector>

struct VertexData {
	DirectX::XMFLOAT3 pos;		// ç¿ïW
	DirectX::XMFLOAT4 normal;	// ñ@ê¸
	DirectX::XMFLOAT2 uv;		// UVç¿ïW
};

using VertexContainer = std::vector<VertexData>;

void loadFBXConvertToVertexData(const char* filename, VertexContainer & outVertexData);