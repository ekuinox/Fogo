#include <utility>
#include "./CallableComponent.h"

using Fogo::Game::CallableComponent;

CallableComponent::CallableComponent(std::function<void(void)> function) : __function(std::move(function)) {
}

void CallableComponent::operator()() const {
	__function();
}