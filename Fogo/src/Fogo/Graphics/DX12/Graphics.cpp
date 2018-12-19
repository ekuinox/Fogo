#include "Graphics.h"
#include "../../Utility/HelperFunctions.h"
#include "../../Utility/Window.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

using namespace Fogo::Graphics::DX12;
using namespace Fogo::Utility;

Graphics * Graphics::instance = nullptr;
std::array<float, 4> Graphics::clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };

auto Graphics::createFactory() -> void {
	ExecOrFail<exception>(CreateDXGIFactory2(enableDebug, IID_PPV_ARGS(factory.GetAddressOf())));
}

auto Graphics::createDevice() -> void {
	ComPtr<IDXGIAdapter3> adapter;
	ExecOrFail<exception>(factory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(adapter.GetAddressOf())));
	ExecOrFail<exception>(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&device)));
}

auto Graphics::createCommandQueue() -> void {
	static constexpr auto commandQueueDesc = D3D12_COMMAND_QUEUE_DESC {
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	ExecOrFail<exception>(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf())));
	ExecOrFail<exception>(CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS));
	ExecOrFail<exception>(device->CreateFence(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(queueFence.GetAddressOf())));
}

auto Graphics::createSwapChain() -> void {
	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC {
		DXGI_MODE_DESC {
			windowSize.width,
			windowSize.height,
			DXGI_RATIONAL { 60, 1 },
			DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED
		},
		DXGI_SAMPLE_DESC { 1, 0	},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		RENDER_TARGET_VIEW_NUMBER,
		windowHandle,
		TRUE,
		DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD,
		DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	ComPtr<IDXGISwapChain> swap_chain;
	ExecOrFail<exception>(factory->CreateSwapChain(commandQueue.Get(), &swapChainDesc, swap_chain.GetAddressOf()));
	ExecOrFail<exception>(swap_chain->QueryInterface(swapChain.GetAddressOf()));

	rtvIndex = swapChain->GetCurrentBackBufferIndex();
}
auto Graphics::createRenderTargetView() -> void {
	renderTargetView = std::make_unique<RenderTargetView>(device, swapChain, RENDER_TARGET_VIEW_NUMBER);
}

auto Graphics::createDepthStencilBuffer() -> void {
	depthStencilView = std::make_unique<DepthStencilView>(device, windowSize.width, windowSize.height);
}

auto Graphics::createCommandList() -> void {
	ExecOrFail<exception>(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator)
	));
	ExecOrFail<exception>(device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	));
}

auto Graphics::waitForPreviousFrame() -> void {
	const UINT64 fence = frames;

	ExecOrFail<exception>(commandQueue->Signal(queueFence.Get(), fence));

	++frames;

	if (queueFence->GetCompletedValue() >= fence) return;

	ExecOrFail<exception>(queueFence->SetEventOnCompletion(fence, fenceEvent));

	WaitForSingleObject(fenceEvent, INFINITE);
}

auto Graphics::setResourceBarrier(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) -> void {
	const auto & resourceBarrier = D3D12_RESOURCE_BARRIER {
		D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_TRANSITION_BARRIER {
			renderTargetView->getTarget(rtvIndex).Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			beforeState,
			afterState
		}
	};

	commandList->ResourceBarrier(1, &resourceBarrier);
}

auto Graphics::populateCommandList() -> void {
	//リソースの状態をプレゼント用からレンダーターゲット用に変更
	setResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//深度バッファとレンダーターゲットのクリア
	commandList->ClearDepthStencilView(depthStencilView->getHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandList->ClearRenderTargetView(renderTargetView->getTargetHandle(rtvIndex), clearColor.data(), 0, nullptr);
	
	// この辺から

	//ビューポートとシザー矩形の設定
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	//レンダーターゲットの設定
	commandList->OMSetRenderTargets(1, &renderTargetView->getTargetHandle(rtvIndex), TRUE, &depthStencilView->getHandle());

	for (const auto & renderer : renderers) renderer(commandList.Get());

	// このへんまでがあれなんだとおもう

	//リソースの状態をレンダーターゲットからプレゼント用に変更
	setResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	ExecOrFail<exception>(commandList->Close());
}

auto Graphics::render() -> void {
	populateCommandList();

	ID3D12CommandList * const commandLists = commandList.Get();
	commandQueue->ExecuteCommandLists(1, &commandLists);

	//実行したコマンドの終了待ち
	waitForPreviousFrame();

	ExecOrFail<exception>(commandAllocator->Reset());
	ExecOrFail<exception>(commandList->Reset(commandAllocator.Get(), nullptr));
	ExecOrFail<exception>(swapChain->Present(1, 0));

	//カレントのバックバッファのインデックスを取得する
	rtvIndex = swapChain->GetCurrentBackBufferIndex();

	renderers.clear();
}

Graphics::Graphics()
	: windowHandle(Window::GetWindowHandle()), windowSize({Window::GetWidth(), Window::GetHeight()}), frames(0), rtvIndex(0) {
	createFactory();
	createDevice();
	createCommandQueue();
	createSwapChain();
	createRenderTargetView();
	createDepthStencilBuffer();
	createCommandList();

	viewport = D3D12_VIEWPORT {
		0.0f,
		0.0f,
		static_cast<float>(windowSize.width),
		static_cast<float>(windowSize.height),
		0.0,
		1.0f
	};

	scissorRect = D3D12_RECT {
		0, 0, static_cast<LONG>(windowSize.width), static_cast<LONG>(windowSize.height)
	};
}

auto Graphics::Create() -> Graphics &
{
	if (instance == nullptr) instance = new Graphics();
	return * instance;
}

auto Graphics::Destroy() -> void
{
	delete instance;
	instance = nullptr;
}

auto Graphics::GetInstance() -> Graphics &
{
	return * instance;
}

auto Graphics::Render(const std::function<void(ComPtr<ID3D12GraphicsCommandList>)> & renderer) -> void {
	GetInstance().renderers.emplace_back(renderer);
}

auto Graphics::Render() -> void {
	GetInstance().render();
}

auto Graphics::GetDevice() -> ComPtr<ID3D12Device> {
	return GetInstance().device;
}

auto Graphics::GetCommandList() -> ComPtr<ID3D12GraphicsCommandList> {
	return GetInstance().commandList;
}

auto Graphics::CompileVertexShader(LPCWSTR fileName, UINT compileFlag, const char * entryFunc, const char * target) -> ComPtr<ID3DBlob> {
	ComPtr<ID3DBlob> shader;
	ComPtr<ID3DBlob> error;
	try {
		ExecOrFail(D3DCompileFromFile(
			fileName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryFunc,
			target,
			compileFlag,
			0,
			shader.GetAddressOf(),
			error.GetAddressOf()
		));
	} catch (std::exception e) {
		throw std::exception(static_cast<LPSTR>(error->GetBufferPointer()));
	}
	
	return shader;
}

auto Graphics::CompilePixelShader(LPCWSTR fileName, UINT compileFlag, const char * entryFunc, const char * target) -> ComPtr<ID3DBlob> {
	ComPtr<ID3DBlob> shader;
	ComPtr<ID3DBlob> error;
	try {
		ExecOrFail(D3DCompileFromFile(
			fileName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryFunc,
			target,
			compileFlag,
			0,
			shader.GetAddressOf(),
			error.GetAddressOf()
		));
	}
	catch (std::exception e) {
		throw std::exception(static_cast<LPSTR>(error->GetBufferPointer()));
	}

	return shader;
}

auto Graphics::SetClearColor(const std::array<float, 4> & newClearColor) -> void {
	clearColor = newClearColor;
}
