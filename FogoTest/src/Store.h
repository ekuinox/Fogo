#pragma once

#include <Fogo.h>

enum class VertexShader { BOX };
enum class PixelShader { BOX };
enum class TextureType { BOX };
using ResourceStore = Fogo::Utility::MappedStore <
	VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	TextureType, std::shared_ptr<Fogo::Graphics::DX12::Texture>
>;
