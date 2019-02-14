#include "./System.h"
#include "./Scene.h"

using Fogo::Game::System;
using Fogo::Game::Scene;

System * System::__instance = nullptr;

void System::exec() {
	Utility::Time::Start();
	Utility::Input::Update();
	__scenes[__current_key]->update();
	__scenes[__current_key]->render();
	if (__go_next) {
		__go_next = false;
		__scenes[__next_key]->start();
		__finalizer = { __current_key, true };
		// キーの切り替え
		__current_key = __next_key;
		Utility::Time::RegisterTimer("FinalizingScene", 1.0f, [&] {
			__scenes[__finalizer.scene]->stop();
			__scenes[__finalizer.scene]->finalize();
			__finalizer.isFinaliing = false;
		});
	}
	Utility::Time::Stop();
	Utility::Time::CheckTimers();
}

void System::onNext() {
	if (__current_key == __next_key) { // 次のシーンキーが切り替えられていない
		Utility::PubSub<Event, void>::Publish(Event::End);
		return;
	}

	// 初期化されてなかったら初期化しましょう
	if (__scenes[__next_key]->getState() != Scene::State::Initialized) {
		__scenes[__next_key]->initialize();
	}

	__go_next = true;
}

void System::onEnd() {
	// Endをサブスクライブしているのが自分自身だけなら
	if (Utility::PubSub<Event, void>::GetSubscriberCount(Event::End) == 1) {
		Utility::Window::Stop();
	}
}

void System::onDestroy() {
	__is_thread_running = false;
	if (__thread.joinable()) __thread.join();
	Utility::Input::Finalize();
}

System::System(Key firstKey, std::unordered_map<Key, std::shared_ptr<Scene>> scenes)
	: __current_key(firstKey), __next_key(firstKey), __is_thread_running(true), __scenes(std::move(scenes)) {
	Utility::PubSub<Event, void>::RegisterSubscriber(Event::Next, [&] { onNext(); });
	Utility::PubSub<Event, void>::RegisterSubscriber(Event::End, [&] { onEnd(); });

	// 最初のシーンを初期化して開始
	__scenes[__current_key]->initialize();
	__scenes[__current_key]->start();

	Utility::Input::Initialize();
	__thread = std::thread([&] {
		while (__is_thread_running) { exec(); }
	});
}

System::~System() {
	onDestroy();
}

void System::Create(Key firstKey, const std::unordered_map<Key, std::shared_ptr<Scene>> & scenes) {
	if (!__instance) __instance = new System(firstKey, scenes);
}

void System::Destroy() {
	delete __instance;
	__instance = nullptr;
}

void System::SetNext(Key key) {
	__instance->__next_key = key;
}

void System::LoadNext() {
	// 前のシーンの終了中にやるなバカ
	if (__instance->__finalizer.isFinaliing) return;

	__instance->__load_next_scene = std::thread([&] {
		__instance->__scenes[__instance->__next_key]->initialize();
	});
}

void System::LoadNextSync() {
	// 前のシーンの終了中にやるなバカ
	if (__instance->__finalizer.isFinaliing) return;

	__instance->__scenes[__instance->__next_key]->initialize();
}

bool System::IsNextSceneInitialized() {
	const auto result = __instance->__scenes[__instance->__next_key]->getState() == Scene::State::Initialized;
	if (result && __instance->__load_next_scene.joinable()) {
		__instance->__load_next_scene.join();
	}
	return result;
}
