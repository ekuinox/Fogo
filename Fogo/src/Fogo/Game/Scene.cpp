#include "./Scene.h"

using namespace Fogo::Game;

auto Scene::update() -> void {
	for (const auto & component : components) {
		component->update();
	}
}

auto Scene::render() const -> void {
	std::vector<std::function<void(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>)>> renderers;
	renderers.clear();
	for (const auto & component : components) {
		renderers.emplace_back([&](Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) { component->render(commandList); });
	}
	Graphics::DX12::Graphics::Render(renderers);
}
