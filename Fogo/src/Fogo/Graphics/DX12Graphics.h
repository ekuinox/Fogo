#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <stdexcept>
#include <vector>
#include <functional>

namespace Fogo {
	class DX12Graphics
	{
	private:
		template <typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // Types
		struct WindowSize { UINT width, height; };
		struct exception : public std::exception { exception(const char * message) : std::exception(message) {} };

	private: // private variables
		static constexpr unsigned char RTV_NUM = 2;
		static constexpr UINT enableDebug = 0;

		static DX12Graphics * instance;

		HWND windowHandle;
		WindowSize windowSize;

		UINT64 frames;
		UINT rtvIndex;

		ComPtr<IDXGIFactory4> factory;
		ComPtr<IDXGIAdapter3> adapter;
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;

		HANDLE fenceEvent;
		ComPtr<ID3D12Fence> queueFence;

		ComPtr<IDXGISwapChain3> swapChain;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		ComPtr<ID3D12Resource> renderTarget[RTV_NUM];
		ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[RTV_NUM];

		ComPtr<ID3D12Resource> depthBuffer;
		ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12RootSignature> rootSignature;

		D3D12_RECT scissorRect;
		D3D12_VIEWPORT viewport;

	private: // private functions
		auto createFactory() -> void;
		auto createDevice() -> void;
		auto createCommandQueue() -> void;
		auto createSwapChain() -> void;
		auto createRenderTargetView() -> void;
		auto createDepthStencilBuffer() -> void;
		auto createCommandList() -> void;
		auto waitForPreviousFrame() -> void;
		auto setResourceBarrier(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) -> void;
		auto populateCommandList(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>> & renderers) -> void;
		auto getDevice() const -> ID3D12Device *;
		auto getCommandList() const -> ID3D12GraphicsCommandList *;
		auto render(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>> & renderers) -> void;
		DX12Graphics(HWND hwnd, const WindowSize & windowSize);

	public: // functions
		DX12Graphics(const DX12Graphics &) = delete;
		auto operator=(const DX12Graphics &) -> DX12Graphics & = delete;
		static auto Create(HWND hwnd, const WindowSize & windowSize) -> DX12Graphics &;
		static auto Destroy() -> void;
		static auto GetInstance() -> DX12Graphics &;
		static auto Render(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>> & renderers) -> void;
		static auto GetDevice() -> ID3D12Device *;
	};
}