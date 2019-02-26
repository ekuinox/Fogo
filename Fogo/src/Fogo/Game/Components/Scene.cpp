#include "./Scene.h"
#include "./Updater.h"
#include "./Renderer.h"
#include "../../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

template <typename T>
void RecursiveExecute(Component & parent) {
	if (parent.getChildrenSize() < 1) return;
	parent.execute<Component>([](Component & component) {
		RecursiveExecute<T>(component);
		component.execute<T>([](T & child) {
			child();
		});
	});
}

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
	RecursiveExecute<Updater>(**get<Component>(uuid));
}

void Scene::render() const {
	RecursiveExecute<Renderer>(**get<Component>(uuid));
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
