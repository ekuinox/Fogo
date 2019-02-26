#include "./Starter.h"

using Fogo::Game::Starter;

Starter::Starter(std::function<void(void)> function) : CallableComponent(function) {

}
