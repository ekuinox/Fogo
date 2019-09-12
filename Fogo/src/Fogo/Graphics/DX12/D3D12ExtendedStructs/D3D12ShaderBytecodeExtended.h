#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12ShaderBytecodeExtended : D3D12_SHADER_BYTECODE {
		using Super = D3D12_SHADER_BYTECODE;
		using This = D3D12ShaderBytecodeExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}

		constexpr auto withShaderBytecode(const void * value) const -> This {
			return Super {
				value,
				BytecodeLength
			};
		}

		constexpr auto withBytecodeLength(const SIZE_T & value) const -> This {
			return Super {
				pShaderBytecode,
				value
			};
		}

		static auto FromBlob(ID3DBlob * blob) -> This {
			return Super {
				blob->GetBufferPointer(),
				blob->GetBufferSize()
			};
		}
	};

}
