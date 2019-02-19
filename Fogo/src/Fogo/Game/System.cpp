#include "./System.h"
#include "./Scene.h"

using Fogo::Game::System;
using Fogo::Game::Scene;
using Fogo::Utility::TaskScheduler;
using Fogo::Utility::Time;
using Fogo::Utility::Input;
using Fogo::Utility::PubSub;
using Fogo::Utility::Window;

System * System::__instance = nullptr;

void System::exec() {
	const auto & currentScene = get<Scene>(__current_key);
	if (!currentScene) return;

	Time::Start();
	Input::Update();
	
	currentScene->update();
	currentScene->render();

	if (__go_next) {
		const auto & nextScene = get<Scene>(__next_key);
		if (nextScene) {
			__go_next = false;
			nextScene->start();
			__finalizer = { __current_key, true };
			// キーの切り替え
			__current_key = __next_key;
			Time::RegisterTimer("FinalizingScene", 1.0f, [&] {
				const auto & finalizeScene = get<Scene>(__finalizer.scene);
				finalizeScene->stop();
				finalizeScene->finalize();
				__finalizer.isFinaliing = false;
			});
		}
	}

	Time::Stop();
	Time::CheckTimers();
	TaskScheduler::ExecTasks();
}

void System::onNext() {
	if (__current_key == __next_key) { // 次のシーンキーが切り替えられていない
		PubSub<Event, void>::Publish(Event::End);
		return;
	}

	// 初期化されてなかったら初期化しましょう
	const auto & nextScene = get<Scene>(__next_key);
	if (!nextScene) return;

	if (nextScene->getState() != Scene::State::Initialized) {
		nextScene->initialize();
	}

	__go_next = true;
}

void System::onEnd() {
	// Endをサブスクライブしているのが自分自身だけなら
	if (PubSub<Event, void>::GetSubscriberCount(Event::End) == 1) {
		Window::Stop();
	}
}

void System::onDestroy() {
	__is_thread_running = false;
	if (__thread.joinable()) __thread.join();
	for (const auto & key : keys) {
		if (const auto & scene = Store::Get<Scene>(get<Scene>(key)->uuid)) {
			scene.get()->destroyIndex(key);
		}
	}
	Input::Finalize();
	TaskScheduler::Destroy();
}

System::System(Key firstKey, std::unordered_map<Key, Scene*> scenes)
	: __current_key(firstKey), __next_key(firstKey), __is_thread_running(true) {
	PubSub<Event, void>::RegisterSubscriber(Event::Next, [&] { onNext(); });
	PubSub<Event, void>::RegisterSubscriber(Event::End, [&] { onEnd(); });
	
	keys.reserve(scenes.size());
	for (auto & [key, scene] : scenes) {
		bind<Scene>(scene).makeIndex(key);
		keys.emplace_back(key);
	}

	// 最初のシーンを初期化して開始
	const auto & currentScene = get<Scene>(__current_key);
	currentScene->initialize();
	currentScene->start();

	Input::Initialize();
	TaskScheduler::Create();
	__thread = std::thread([&] {
		while (__is_thread_running) { exec(); }
	});
}

System::System(const Key & firstKey, const std::function<void(System &)> & createScenes)
	: __current_key(firstKey), __next_key(firstKey), __is_thread_running(true) {
		PubSub<Event, void>::RegisterSubscriber(Event::Next, [&] { onNext(); });
		PubSub<Event, void>::RegisterSubscriber(Event::End, [&] { onEnd(); });

		std::cout << "this->uuid: " << this->uuid << std::endl;

		createScenes(*this);
		
		std::cout << "execute" << std::endl;
		execute<Scene>([](Scene & scene) {
			std::cout << scene.uuid << std::endl;
		});

		// 最初のシーンを初期化して開始
		const auto & currentScene = get<Scene>(__current_key);

		// Sceneの継承クラスに対してインデックスをつけているのでSceneで引っぱってきても返却されない　おそらく

		currentScene->initialize();
		currentScene->start();

		Input::Initialize();
		TaskScheduler::Create();
		__thread = std::thread([&] {
			while (__is_thread_running) { exec(); }
		});
}

System::~System() {
	onDestroy();
}

void System::Create(Key firstKey, const std::unordered_map<Key, Scene*> & scenes) {
	if (!__instance) __instance = new System(firstKey, scenes);
}

void System::Create(const Key & firstKey, const std::function<void(System &)> & createScenes) {
	if (!__instance) __instance = new System(firstKey, createScenes);
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

	TaskScheduler::AddTask(TaskScheduler::Priority::Highest, [&] {
		__instance->get<Scene>(__instance->__next_key)->initialize();
	});
}

void System::LoadNextSync() {
	// 前のシーンの終了中にやるなバカ
	if (__instance->__finalizer.isFinaliing) return;

	__instance->get<Scene>(__instance->__next_key)->initialize();
}

bool System::IsNextSceneInitialized() {
	const auto result = __instance->get<Scene>(__instance->__next_key)->getState() == Scene::State::Initialized;
	if (result && __instance->__load_next_scene.joinable()) {
		__instance->__load_next_scene.join();
	}
	return result;
}

System & System::GetInstance() {
	return * __instance;
}

Scene & System::GetCurrentScene() {
	return *(__instance->get<Scene>(__instance->__current_key).get().value());
}

Scene & System::GetScene(const Key & key) {
	return *(__instance->get<Scene>(key).get().value());
}
