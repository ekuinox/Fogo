#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {

	struct D3D12ResourceDescExtended : D3D12_RESOURCE_DESC {
		using Super = D3D12_RESOURCE_DESC;
		using This = D3D12ResourceDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		
		constexpr auto withDimension(const D3D12_RESOURCE_DIMENSION & newDimension) const -> This {
			return Super {
				newDimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withAlignment(const UINT64 & newAlignment) const -> This {
			return Super {
				this->Dimension,
				newAlignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withWidth(const UINT64 & newWidth) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				newWidth,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withHeight(const UINT & newHeight) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				newHeight,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withDepthOrArraySize(const UINT16 & newDepthOrArraySize) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				newDepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withMipLevels(const UINT16 & newMipLevels) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				newMipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withFormat(const DXGI_FORMAT & newFormat) const -> This {
			return Super{
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				newFormat,
				this->SampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withSampleDesc(const DXGI_SAMPLE_DESC & newSampleDesc) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				newSampleDesc,
				this->Layout,
				this->Flags
			};
		}
		
		constexpr auto withLayout(const D3D12_TEXTURE_LAYOUT & newLayout) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				newLayout,
				this->Flags
			};
		}
		
		constexpr auto withFlags(const D3D12_RESOURCE_FLAGS & newFlags) const -> This {
			return Super {
				this->Dimension,
				this->Alignment,
				this->Width,
				this->Height,
				this->DepthOrArraySize,
				this->MipLevels,
				this->Format,
				this->SampleDesc,
				this->Layout,
				newFlags
			};
		}
	};

}
