#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "./Texture.h"

namespace Fogo::Graphics::DX12 {
	class FBXModel {
	public:
		struct Properties {
			std::shared_ptr<ID3D10Blob> vertexShader, pixelShader;
			LPCWSTR textureDirectory;
			Properties & setTextureDirectory(LPCWSTR newTextureDirectory) { textureDirectory = newTextureDirectory; return *this; }
		};
		struct Vertex {
			DirectX::XMFLOAT3 position;		// ç¿ïW
			DirectX::XMFLOAT4 normal;		// ñ@ê¸
			DirectX::XMFLOAT2 texture;		// UVç¿ïW
		};
		struct Material {
			DirectX::XMFLOAT3 emissive, ambient, diffuse, specular;
			float transparency, shininess;
			std::shared_ptr<Texture> texture;
			std::string texturePath;
		};
		struct Mesh {
			std::vector<Vertex> vertexes;
			Material material;
		};
	private:
		std::vector<Mesh> __meshes;
		Properties __properties;

		void loadModel(const char * fileName) noexcept;

	public:
		FBXModel(const char * fileName, const Properties & properties = {});
		const std::vector<Mesh> & getMeshes() const;

	};
}
