#pragma once

#include <Fogo.h>

class Model : public Fogo::Component {
	const char * modelFile;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
	std::unique_ptr<Fogo::FBXModel> model;

	void initialize();
	void render() const;

public:
	Model(const char * modelFile, Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, Microsoft::WRL::ComPtr<ID3DBlob> pixelShader);
	DirectX::XMFLOAT3 getPosition() const;

	void setPosition(DirectX::XMFLOAT3 position) const;
	DirectX::XMFLOAT4X4 getWorldMatrix() const;
	void setWorldMatrix(DirectX::XMFLOAT4X4 & matrix) const;
	void setWorldMatrix(DirectX::XMMATRIX & matrix) const;

	DirectX::XMFLOAT3 translation, rotation;
	

};
