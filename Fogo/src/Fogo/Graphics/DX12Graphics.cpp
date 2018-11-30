#include "DX12Graphics.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Fogo::DX12Graphics * Fogo::DX12Graphics::instance = nullptr;

auto Fogo::DX12Graphics::createFactory() -> void {
	if (FAILED(CreateDXGIFactory2(enableDebug, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		throw exception("[DX12Graphics] createFactory error");
	}
}

auto Fogo::DX12Graphics::createDevice() -> void {
	ComPtr<IDXGIAdapter3> adapter;
	if (FAILED(factory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(adapter.GetAddressOf()))))
	{
		throw exception("[DX12Graphics] createDevice error");
	}

	if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&device))))
	{
		throw exception("[DX12Graphics] createDevice error");
	}
}

auto Fogo::DX12Graphics::createCommandQueue() -> void {
	static constexpr auto commandQueueDesc = D3D12_COMMAND_QUEUE_DESC {
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()))))
	{
		throw exception("[DX12Graphics] createCommandQueue error");
	}

	if ((CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS)) == NULL)
	{
		throw std::exception("[DX12Graphics] createCommandQueue error");
	}

	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(queueFence.GetAddressOf()))))
	{
		throw exception("[DX12Graphics] createCommandQueue error");
	}
}

auto Fogo::DX12Graphics::createSwapChain() -> void {
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
		RTV_NUM,
		windowHandle,
		TRUE,
		DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD,
		DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	ComPtr<IDXGISwapChain> swap_chain;

	if (FAILED(factory->CreateSwapChain(commandQueue.Get(), &swapChainDesc, swap_chain.GetAddressOf())))
	{
		throw exception("[DX12Graphics] createSwapChain error");
	}

	if (FAILED(swap_chain->QueryInterface(swapChain.GetAddressOf())))
	{
		throw exception("[DX12Graphics] createSwapChain error");
	}

	rtvIndex = swapChain->GetCurrentBackBufferIndex();
}
auto Fogo::DX12Graphics::createRenderTargetView() -> void {
	static constexpr auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC {
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		RTV_NUM,
		D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	if (FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvDescriptorHeap.GetAddressOf()))))
	{
		throw exception("[DX12Graphics] createRenderTargetView error");
	}

	const auto size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (auto i = 0; i < RTV_NUM; ++i)
	{
		if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget[i]))))
		{
			throw exception("[DX12Graphics] createRenderTargetView error");
		}

		rtvHandle[i] = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle[i].ptr += size * i;
		device->CreateRenderTargetView(renderTarget[i].Get(), nullptr, rtvHandle[i]);
	}
}

auto Fogo::DX12Graphics::createDepthStencilBuffer() -> void {
	static constexpr auto descriptorHeapDesc = D3D12_DESCRIPTOR_HEAP_DESC {
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	if (FAILED(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&dsvDescriptorHeap))))
	{
		throw exception("[DX12Graphics] createDepthStencilBuffer error");
	}

	static constexpr auto heapProperties = D3D12_HEAP_PROPERTIES {
		D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	const auto & resourceDesc = D3D12_RESOURCE_DESC {
		D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		windowSize.width,
		windowSize.height,
		1,
		0,
		DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS,
		DXGI_SAMPLE_DESC { 1, 0 },
		D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	auto clearValue = D3D12_CLEAR_VALUE {
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
	};
	clearValue.DepthStencil = D3D12_DEPTH_STENCIL_VALUE { 1.0f, 0 };

	if (FAILED(device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuffer)
	)))
	{
		throw exception("[DX12Graphics] createDepthStencilBuffer error");
	}

	auto dsvDesc = D3D12_DEPTH_STENCIL_VIEW_DESC {
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
		D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D,
		D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE
	};
	dsvDesc.Texture2D.MipSlice = 0;

	dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHandle);
}

auto Fogo::DX12Graphics::createCommandList() -> void {
	if (FAILED(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator)
	)))
	{
		throw exception("[DX12Graphics] createCommandList");
	}

	if (FAILED(device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	)))
	{
		throw exception("[DX12Graphics] createCommandList");
	}
}

auto Fogo::DX12Graphics::waitForPreviousFrame() -> void {
	const UINT64 fence = frames;
	if (FAILED(commandQueue->Signal(queueFence.Get(), fence)))
	{
		throw exception("[DX12Graphics] waitForPreviousFrame");
	}

	++frames;

	if (queueFence->GetCompletedValue() >= fence) return;

	if (FAILED(queueFence->SetEventOnCompletion(fence, fenceEvent)))
	{
		throw exception("[DX12Graphics] waitForPreviousFrame");
	}

	WaitForSingleObject(fenceEvent, INFINITE);
}

auto Fogo::DX12Graphics::setResourceBarrier(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) -> void {
	const auto & resourceBarrier = D3D12_RESOURCE_BARRIER {
		D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE,
		D3D12_RESOURCE_TRANSITION_BARRIER {
			renderTarget[rtvIndex].Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			beforeState,
			afterState
		}
	};

	commandList->ResourceBarrier(1, &resourceBarrier);
}

auto Fogo::DX12Graphics::populateCommandList(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>> & renderers) -> void {
	static constexpr FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//���\�[�X�̏�Ԃ��v���[���g�p���烌���_�[�^�[�Q�b�g�p�ɕύX
	setResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//�[�x�o�b�t�@�ƃ����_�[�^�[�Q�b�g�̃N���A
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandList->ClearRenderTargetView(rtvHandle[rtvIndex], clearColor, 0, nullptr);

	// ���̕ӂ���

	//�r���[�|�[�g�ƃV�U�[��`�̐ݒ�
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	//�����_�[�^�[�Q�b�g�̐ݒ�
	commandList->OMSetRenderTargets(1, &rtvHandle[rtvIndex], TRUE, &dsvHandle);

	for (const auto & renderer : renderers) renderer(commandList.Get());

	// ���̂ւ�܂ł�����Ȃ񂾂Ƃ�����

	//���\�[�X�̏�Ԃ������_�[�^�[�Q�b�g����v���[���g�p�ɕύX
	setResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	if (FAILED(commandList->Close()))
	{
		throw exception("[DX12Graphics] populateCommandList error");
	}
}

auto Fogo::DX12Graphics::getDevice() const -> ID3D12Device * {
	return device.Get();

}

auto Fogo::DX12Graphics::getCommandList() const -> ID3D12GraphicsCommandList * {
	return commandList.Get();
}

auto Fogo::DX12Graphics::render(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>> & renderers) -> void {
	populateCommandList(renderers);

	ID3D12CommandList * const commandLists = commandList.Get();
	commandQueue->ExecuteCommandLists(1, &commandLists);

	//���s�����R�}���h�̏I���҂�
	waitForPreviousFrame();

	if (FAILED(commandAllocator->Reset()))
	{
		throw exception("[DX12Graphics] render error");
	}

	if (FAILED(commandList->Reset(commandAllocator.Get(), nullptr)))
	{
		throw exception("[DX12Graphics] render error");
	}

	if (FAILED(swapChain->Present(1, 0)))
	{
		throw exception("[DX12Graphics] render error");
	}

	//�J�����g�̃o�b�N�o�b�t�@�̃C���f�b�N�X���擾����
	rtvIndex = swapChain->GetCurrentBackBufferIndex();
}

Fogo::DX12Graphics::DX12Graphics(HWND hwnd, const WindowSize & windowSize)
	: windowHandle(hwnd), windowSize(windowSize), frames(0), rtvIndex(0) {
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

auto Fogo::DX12Graphics::Create(HWND hwnd, const WindowSize & windowSize) -> DX12Graphics &
{
	if (instance == nullptr) instance = new DX12Graphics(hwnd, windowSize);
	return * instance;
}

auto Fogo::DX12Graphics::Destroy() -> void
{
	delete instance;
	instance = nullptr;
}

auto Fogo::DX12Graphics::GetInstance() -> DX12Graphics &
{
	return * instance;
}

auto Fogo::DX12Graphics::Render(const std::vector<std::function<void(ID3D12GraphicsCommandList*)>>& renderers) -> void {
	GetInstance().render(renderers);
}

auto Fogo::DX12Graphics::GetDevice() -> ID3D12Device * {
	return GetInstance().getDevice();
}

