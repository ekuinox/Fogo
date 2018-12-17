#include "./Scene.h"
#include "../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

auto Scene::initialize() -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Created) component->initialize();
	}
	ComponentInterface::initialize();
}

auto Scene::start() -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Initialized) component->start();
	}
	ComponentInterface::start();
}


auto Scene::update() -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Started) component->update();
	}
	ComponentInterface::update();
}

auto Scene::render() const -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Started) component->render();
	}
	::Graphics::Render();
	ComponentInterface::render();
}

auto Scene::stop() -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Started) component->stop();
	}
	ComponentInterface::stop();
}

auto Scene::finalize() -> void {
	for (const auto & component : components) {
		if (component->getState() == State::Initialized) component->finalize();
	}
	components.clear();
	ComponentInterface::finalize();
}
