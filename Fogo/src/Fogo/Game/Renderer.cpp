#include "./Renderer.h"

using Fogo::Game::Renderer;

Renderer::Renderer(std::function<void()> function) : CallableComponent(function) {
}
