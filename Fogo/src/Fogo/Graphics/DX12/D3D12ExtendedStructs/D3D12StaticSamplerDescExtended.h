#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {
	struct D3D12StaticSamplerDescExtended : D3D12_STATIC_SAMPLER_DESC {
		using Super = D3D12_STATIC_SAMPLER_DESC;
		using This = D3D12StaticSamplerDescExtended;
		constexpr This(const Super  & raw) : Super(raw) {}
		constexpr This() : Super() {}

		constexpr auto withFilter(const D3D12_FILTER & newFilter) const -> This {
			return Super {
				newFilter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withAddressU(const D3D12_TEXTURE_ADDRESS_MODE & newAddressU) const -> This {
			return Super {
				Filter,
				newAddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withAddressV(const D3D12_TEXTURE_ADDRESS_MODE & newAddressV) const -> This {
			return Super {
				Filter,
				AddressU,
				newAddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withAddressW(const D3D12_TEXTURE_ADDRESS_MODE & newAddressW) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				newAddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withMipLODBias(const FLOAT & newMipLODBias) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				newMipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withMaxAnisotropy(const UINT & newMaxAnisotropy) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				newMaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withComparisonFunc(const D3D12_COMPARISON_FUNC & newComparisonFunc) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				newComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withBorderColor(const D3D12_STATIC_BORDER_COLOR & newBorderColor) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				newBorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withMinLOD(const FLOAT & newMinLOD) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				newMinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withMaxLOD(const FLOAT & newMaxLOD) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				newMaxLOD,
				ShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withShaderRegister(const UINT & newShaderRegister) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				newShaderRegister,
				RegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withRegisterSpace(const UINT & newRegisterSpace) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				newRegisterSpace,
				ShaderVisibility
			};
		}

		constexpr auto withShaderVisibility(const D3D12_SHADER_VISIBILITY & newShaderVisibility) const -> This {
			return Super {
				Filter,
				AddressU,
				AddressV,
				AddressW,
				MipLODBias,
				MaxAnisotropy,
				ComparisonFunc,
				BorderColor,
				MinLOD,
				MaxLOD,
				ShaderRegister,
				RegisterSpace,
				newShaderVisibility
			};
		}
	};

}
