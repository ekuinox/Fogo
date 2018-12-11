#include "Fogo.h"

void Fogo::Initialize(const Properties & properties) {
	Utility::Window::Create(properties.window);
	Graphics::DX12::Graphics::Create(Utility::Window::GetHandle(), { properties.window.width, properties.window.height });
	Game::GameController::Create(properties.scenes);
	Utility::Window::GetInstance().run();
}

void Fogo::Finalize() {
	Utility::Window::Destroy();
	Graphics::DX12::Graphics::Destroy();
}
