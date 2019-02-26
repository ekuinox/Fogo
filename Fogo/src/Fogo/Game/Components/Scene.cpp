#include "./Scene.h"
#include "./Updater.h"
#include "./Renderer.h"
#include "./Initializer.h"
#include "./Finalizer.h"
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

Scene::Scene() : __state(State::Created) {

}

void Scene::initialize() {
	RecursiveExecute<Initializer>(**get<Component>(uuid));
	__state = State::Initialized;
}

void Scene::start() {
	__state = State::Started;
}

void Scene::update() {
	RecursiveExecute<Updater>(**get<Component>(uuid));
}

Scene::State Scene::getState() const {
	return __state;
}

void Scene::render() const {
	RecursiveExecute<Renderer>(**get<Component>(uuid));
	::Graphics::Render();
}

void Scene::stop() {

}

void Scene::finalize() {
	RecursiveExecute<Finalizer>(**get<Component>(uuid));
	__state = State::Created;
}
