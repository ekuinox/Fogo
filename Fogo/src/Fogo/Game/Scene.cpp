#include "./Scene.h"
#include "../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

void Scene::initialize() {
	execute<LifeCycled>([](LifeCycled & component) {
		component.initialize();
	});
	LifeCycled::initialize();
}

void Scene::start() {
	execute<LifeCycled>([](LifeCycled & component) {
		component.start();
	});
	LifeCycled::start();
}

void Scene::update() {
	execute<Updatable>([](Updatable & component) {
		component.update();
	});
}

void Scene::render() const {
	execute<Renderable>([](Renderable & component) {
		component.render();
	});
	::Graphics::Render();
}

void Scene::stop() {
	execute<LifeCycled>([](LifeCycled & component) {
		component.stop();
	});
	LifeCycled::stop();
}

void Scene::finalize() {
	execute<LifeCycled>([](LifeCycled & component) {
		component.finalize();
	});
	LifeCycled::finalize();
}
