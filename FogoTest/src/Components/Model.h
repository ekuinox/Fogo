#pragma once

#include <Fogo.h>

class Model : public Fogo::Game::Component {
	const char * modelFile;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, pixelShader;
	std::unique_ptr<Fogo::Graphics::DX12::FBXModel> model;

	void initialize();
	void update();
	void render();

public:
	Model(const char * modelFile, Microsoft::WRL::ComPtr<ID3DBlob> vertexShader, Microsoft::WRL::ComPtr<ID3DBlob> pixelShader);

};
