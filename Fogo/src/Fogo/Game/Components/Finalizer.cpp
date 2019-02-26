#include "./Finalizer.h"

using Fogo::Game::Finalizer;

Finalizer::Finalizer(std::function<void(void)> function) : CallableComponent(function) {

}
