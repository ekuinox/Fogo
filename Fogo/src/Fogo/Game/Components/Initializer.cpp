#include "./Initializer.h"

using Fogo::Game::Initializer;

Initializer::Initializer(std::function<void(void)> function) : CallableComponent(function) {

}
