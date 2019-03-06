#include "./Scene.h"
#include "./CallableComponents.h"
#include "../../Graphics.h"

using namespace Fogo;

template <typename T>
void RecursiveExecute(Component & parent) {
	parent.execute<T>([](T & child) {
		child();
	});
	parent.execute<Component>([](Component & component) {
		RecursiveExecute<T>(component);
	});
}

Scene::Scene() : __state(State::Created) {

}

void Scene::initialize() {
	RecursiveExecute<Initializer>(**get<Component>(uuid));
	__state = State::Initialized;
}

void Scene::start() {
	RecursiveExecute<Starter>(**get<Component>(uuid));
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
	RecursiveExecute<Stopper>(**get<Component>(uuid));
	__state = State::Initialized;
}

void Scene::finalize() {
	RecursiveExecute<Finalizer>(**get<Component>(uuid));
	__state = State::Created;
}
