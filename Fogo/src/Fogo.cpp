#include "Fogo.h"

void Fogo::Initialize(const Properties & properties) {
	Utility::Window::Create(properties.window);
	Graphics::DX12::Graphics::Create();
//	Game::System::Create(properties.first_key, properties.scenes);
	Game::System::Create(properties.first_key, properties.createScenes);
	Utility::Window::Run();
}

void Fogo::Finalize() {
	Utility::Window::Destroy();
	Graphics::DX12::Graphics::Destroy();
}
