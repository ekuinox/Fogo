#pragma once

#include "Fogo.h"
#include "../Square.h"
#include "../Car.h"
#include "../FBXSample.h"
#include <iostream>

class MainScene : public Fogo::Game::Scene {
private:
	struct InputDebugger : Fogo::Game::ComponentInterface {
		void update() override {
			if (Fogo::Utility::Input::GetPress(Fogo::Utility::MouseButton::Right)) {
				std::cout << "‰EƒNƒŠ" << std::endl;
			}
			if (Fogo::Utility::Input::GetPress(Fogo::Utility::MouseButton::Left)) {
				std::cout << "¶ƒNƒŠ" << std::endl;
			}
			const auto move = Fogo::Utility::Input::GetMouseMove();
			if (!(move.x == 0.0f && move.y == 0.0f && move.z == 0.0f)) {
				std::cout << move.x << ", " << move.y << ", " << move.z << std::endl;
			}
		}
	};
public:
	using ComponentStore = Fogo::Utility::TreeStore<
		std::shared_ptr<Fogo::Game::ComponentInterface>, int,
		std::shared_ptr<Square>, std::shared_ptr<Car>, std::shared_ptr<FBXSample>, std::shared_ptr<InputDebugger>>;
	enum class VertexShader { BOX };
	enum class PixelShader { BOX };
	enum class TextureType { BOX };
	using ResourceStore = Fogo::Utility::MappedStore<
		VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
		TextureType, std::shared_ptr<Fogo::Graphics::DX12::Texture>
	>;

	void initialize() override;
};
