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
	if (!(__current_scene_index < __scenes.size())) return;
	__scenes[__current_scene_index]->update();
	if (!(__current_scene_index < __scenes.size())) return;
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
		if (__current_scene_index < __scenes.size()) {
			__scenes[__current_scene_index]->finalize(); // 現在のシーンを終了して
			++__current_scene_index;
			if (__current_scene_index == __scenes.size()) PubSub<Event, void>::Publish(Event::End); // オワっとる
			else __scenes[__current_scene_index]->initialize(); // 次のシーンを初期化する
		}
	});

	PubSub<Event, void>::RegisterSubscriber(Event::MakeSceneIndexFirst, [&] {
		__scenes[__current_scene_index]->finalize(); // 現在のシーンを終了して
		__current_scene_index = 0;
		__scenes[__current_scene_index]->initialize();
	});

	PubSub<Event, void>::RegisterSubscriber(Event::End, [&] {
		// Endをサブスクライブしているのが自分自身だけなら
		if (PubSub<Event, void>::GetSubscriberCount(Event::End) == 1) {
			Window::Stop();
		}
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
