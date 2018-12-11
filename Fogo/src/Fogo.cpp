#include "Fogo.h"

void Fogo::Initialize(const Utility::Window::Properties & properties) {
	Utility::Window::Create(properties);
	Graphics::DX12::Graphics::Create(Utility::Window::GetHandle(), { properties.width, properties.height });
}

void Fogo::Finalize() {
	Utility::Window::Destroy();
	Graphics::DX12::Graphics::Destroy();
}
