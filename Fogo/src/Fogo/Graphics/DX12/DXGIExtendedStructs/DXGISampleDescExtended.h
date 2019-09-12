#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {

	struct DXGISampleDescExtended : DXGI_SAMPLE_DESC {
		using Super = DXGI_SAMPLE_DESC;
		using This = DXGISampleDescExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		constexpr auto withCount(const UINT& newCount) const->This;
		constexpr auto withQuality(const UINT& newQuality) const->This;
	};

	constexpr auto DXGISampleDescExtended::withCount(const UINT& newCount) const -> This {
		return Super {
			newCount,
			this->Quality
		};
	}
	constexpr auto DXGISampleDescExtended::withQuality(const UINT& newQuality) const -> This {
		return Super {
			this->Count,
			newQuality
		};
	}

}
