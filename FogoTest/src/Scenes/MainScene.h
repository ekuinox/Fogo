#pragma once

#include <Fogo.h>
#include "../Square.h"
#include "../FBXSample.h"
#include <iostream>

class MainScene : public Fogo::Game::Scene {
private:
	struct InputDebugger : ComponentInterface {
		// nothing
	};
public:
	using ComponentStore = Fogo::Utility::TreeStore<
		std::shared_ptr<ComponentInterface>, int,
		std::shared_ptr<Square>, std::shared_ptr<FBXSample>, std::shared_ptr<InputDebugger>>;
	enum class VertexShader { BOX };
	enum class PixelShader { BOX };
	enum class TextureType { BOX };
	using ResourceStore = Fogo::Utility::MappedStore<
		VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		TextureType, std::shared_ptr<Fogo::Graphics::DX12::Texture>
	>;

	void initialize() override;
	void update() override;
	void finalize() override;
};
