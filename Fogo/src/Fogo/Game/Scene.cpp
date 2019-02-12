#include "./Scene.h"
#include "../Graphics.h"

using namespace Fogo::Game;
using namespace Fogo::Graphics::DX12;

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
