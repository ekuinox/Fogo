#include "./Stopper.h"

using Fogo::Game::Stopper;

Stopper::Stopper(std::function<void(void)> function) : CallableComponent(function) {

}
