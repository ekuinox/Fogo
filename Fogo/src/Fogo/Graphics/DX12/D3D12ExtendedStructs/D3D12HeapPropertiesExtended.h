#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {

	struct D3D12HeapPropertiesExtended : D3D12_HEAP_PROPERTIES {
		using Super = D3D12_HEAP_PROPERTIES;
		using This = D3D12HeapPropertiesExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		constexpr auto withType(const D3D12_HEAP_TYPE & newType) const -> This;
		constexpr auto withCPUPageProperty(const D3D12_CPU_PAGE_PROPERTY & newCPUPageProperty) const -> This;
		constexpr auto withMemoryPoolPreference(const D3D12_MEMORY_POOL & newMemoryPoolPreference) const -> This;
		constexpr auto withCreationNodeMask(const UINT & newCreationNodeMask) const -> This;
		constexpr auto withVisibleNodeMask(const UINT & newVisibleNodeMask) const -> This;
	};

	constexpr auto D3D12HeapPropertiesExtended::withType(const D3D12_HEAP_TYPE & newType) const -> This
	{
		return Super {
			newType,
			this->CPUPageProperty,
			this->MemoryPoolPreference,
			this->CreationNodeMask,
			this->VisibleNodeMask
		};
	}

	constexpr auto D3D12HeapPropertiesExtended::withCPUPageProperty(const D3D12_CPU_PAGE_PROPERTY & newCPUPageProperty) const -> This
	{
		return Super {
			this->Type,
			newCPUPageProperty,
			this->MemoryPoolPreference,
			this->CreationNodeMask,
			this->VisibleNodeMask
		};
	}

	constexpr auto D3D12HeapPropertiesExtended::withMemoryPoolPreference(const D3D12_MEMORY_POOL & newMemoryPoolPreference) const -> This
	{
		return Super {
			this->Type,
			this->CPUPageProperty,
			newMemoryPoolPreference,
			this->CreationNodeMask,
			this->VisibleNodeMask
		};
	}

	constexpr auto D3D12HeapPropertiesExtended::withCreationNodeMask(const UINT & newCreationNodeMask) const -> This
	{
		return Super {
			this->Type,
			this->CPUPageProperty,
			this->MemoryPoolPreference,
			newCreationNodeMask,
			this->VisibleNodeMask
		};
	}

	constexpr auto D3D12HeapPropertiesExtended::withVisibleNodeMask(const UINT & newVisibleNodeMask) const -> This
	{
		return Super {
			this->Type,
			this->CPUPageProperty,
			this->MemoryPoolPreference,
			this->CreationNodeMask,
			newVisibleNodeMask
		};
	}

}
