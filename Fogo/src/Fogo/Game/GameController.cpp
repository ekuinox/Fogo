#include <utility>
#include "./GameController.h"
#include "../Utility/Time.h"

using namespace Fogo::Game;
using namespace Fogo::Utility;

auto GameController::exec() const -> void {
	Time::Start();
	__scenes[__current_scene_index]->update();
	__scenes[__current_scene_index]->render();
	Time::Stop();
}

GameController::GameController(std::vector<std::shared_ptr<Scene>> scenes) :
	__scenes(std::move(scenes)), __current_scene_index(0), __is_thread_running(true) {
	__thread = std::thread([&] {
		while (__is_thread_running) { exec(); }
	});
	
}

GameController::~GameController() {
	__is_thread_running = false;
	if (__thread.joinable()) __thread.join();
}
