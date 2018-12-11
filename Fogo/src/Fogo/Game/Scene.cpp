#include "./Scene.h"
#include "../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

auto Scene::initialize() -> void {
	
}

auto Scene::update() -> void {
	for (const auto & component : components) {
		component->update();
	}
}

auto Scene::render() const -> void {
	for (const auto & component : components) {
		component->render();
	}
	::Graphics::Render();
}

auto Scene::finalize() -> void {
	
}
