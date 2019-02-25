#include "./Updater.h"

using Fogo::Game::Updater;

Updater::Updater(std::function<void()> function) : CallableComponent(function) {
	
}
