#pragma once

#include "../../../../external/d3dx12.h"

namespace Fogo {

	struct D3D12RootParameterExtended : D3D12_ROOT_PARAMETER {
		using Super = D3D12_ROOT_PARAMETER;
		using This = D3D12RootParameterExtended;
		constexpr This(const Super & raw) : Super(raw) {}
		constexpr This() : Super() {}
		constexpr auto withParameterType(const D3D12_ROOT_PARAMETER_TYPE & newParameterType) const -> This;
		constexpr auto withShaderVisibility(const D3D12_SHADER_VISIBILITY & newShaderVisibility) const->This;
		// withParameterType, withShaderVisibility‚ðæ‚ÉŒÄ‚Ô•K—v‚ª‚ ‚é
		auto withDescriptorTable(const D3D12_ROOT_DESCRIPTOR_TABLE & newDescriptorTable) const -> This;
		auto withConstants(const D3D12_ROOT_CONSTANTS & newConstants) const -> This;
		auto withDescriptor(const D3D12_ROOT_DESCRIPTOR & newDescriptor) const -> This;
	};

	constexpr auto D3D12RootParameterExtended::withParameterType(const D3D12_ROOT_PARAMETER_TYPE & newParameterType) const -> This
	{
		return Super {
			newParameterType,
			this->DescriptorTable,
			this->ShaderVisibility
		};
	}

	constexpr auto D3D12RootParameterExtended::withShaderVisibility(const D3D12_SHADER_VISIBILITY& newShaderVisibility) const -> This
	{
		return Super {
			this->ParameterType,
			this->DescriptorTable,
			newShaderVisibility
		};
	}

	auto D3D12RootParameterExtended::withDescriptorTable(const D3D12_ROOT_DESCRIPTOR_TABLE & newDescriptorTable) const -> This
	{
		Super newInstance = *this;
		newInstance.DescriptorTable = newDescriptorTable;
		return newInstance;
	}

	auto D3D12RootParameterExtended::withConstants(const D3D12_ROOT_CONSTANTS & newConstants) const -> This
	{
		Super newInstance = *this;
		newInstance.Constants = newConstants;
		return newInstance;
	}

	auto D3D12RootParameterExtended::withDescriptor(const D3D12_ROOT_DESCRIPTOR & newDescriptor) const -> This
	{
		Super newInstance = *this;
		newInstance.Descriptor = newDescriptor;
		return newInstance;
	}

}
