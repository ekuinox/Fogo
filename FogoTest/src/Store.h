#pragma once

#include <Fogo.h>

enum class VertexShader { BOX };
enum class PixelShader { BOX };
enum class TextureType { BOX };
using ResourceStore = Fogo::MappedStore <
	VertexShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	PixelShader, Microsoft::WRL::ComPtr<ID3DBlob>,
	TextureType, std::shared_ptr<Fogo::Texture>
>;
