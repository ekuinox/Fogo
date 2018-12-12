#include "./Scene.h"
#include "../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

auto Scene::initialize() -> void {
	for (const auto & component : components) {
		component->initialize();
	}
	ComponentInterface::initialize();
}

auto Scene::start() -> void {
	for (const auto & component : components) {
		component->start();
	}
	ComponentInterface::start();
}


auto Scene::update() -> void {
	for (const auto & component : components) {
		component->update();
	}
	ComponentInterface::update();
}

auto Scene::render() const -> void {
	for (const auto & component : components) {
		component->render();
	}
	::Graphics::Render();
	ComponentInterface::render();
}

auto Scene::stop() -> void {
	for (const auto & component : components) {
		component->stop();
	}
	ComponentInterface::stop();
}

auto Scene::finalize() -> void {
	for (const auto & component : components) {
		component->finalize();
	}
	components.clear();
	ComponentInterface::finalize();
}
