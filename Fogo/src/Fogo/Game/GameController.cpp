#include <utility>
#include "./GameController.h"
#include "../Utility/Time.h"
#include "../Utility/Input.h"
#include "../Utility/Window.h"

using namespace Fogo::Game;
using namespace Fogo::Utility;

GameController * GameController::__instance = nullptr;

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
	PubSub<Event, void>::RegisterSubscriber(Event::NextScene, [&] {
		if (__scenes.size() < __current_scene_index - 1) ++__current_scene_index;
		__scenes[__current_scene_index]->initialize();
	});
	__scenes[__current_scene_index]->initialize();
	Input::Initialize();
	__thread = std::thread([&] {
		while (__is_thread_running) { exec(); }
	});
}

GameController::~GameController() {
	onDestroy();
}

void GameController::Create(const std::vector<std::shared_ptr<Scene>> & scenes) {
	if (!__instance) __instance = new GameController(scenes);
}

void GameController::Destroy() {
	delete __instance;
	__instance = nullptr;
}
