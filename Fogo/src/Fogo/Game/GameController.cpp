#include <utility>
#include "./GameController.h"
#include "../Utility/Time.h"
#include "../Utility/Input.h"
#include "../Utility/Window.h"

using namespace Fogo::Game;
using namespace Fogo::Utility;

auto GameController::exec() const -> void {
	Time::Start();
	Input::Update();
	__scenes[__current_scene_index]->update();
	__scenes[__current_scene_index]->render();
	Time::Stop();
}

auto GameController::onDestroy() -> void {
	__is_thread_running = false;
	if (__thread.joinable()) __thread.join();
	Input::Finalize();
}

GameController::GameController(std::vector<std::shared_ptr<Scene>> scenes) :
	__scenes(std::move(scenes)), __current_scene_index(0), __is_thread_running(true) {
	Window::PubSub::RegisterSubscriber(Window::Event::OnDestroy, [&] { onDestroy(); });
	Input::Initialize();
	__thread = std::thread([&] {
		while (__is_thread_running) { exec(); }
	});
	
}

GameController::~GameController() {
	onDestroy();
}
