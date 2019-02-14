#include "./System.h"
#include "./Scene.h"

using Fogo::Game::System;
using Fogo::Game::Scene;

System * System::__instance = nullptr;

void System::exec() {
	const auto & currentScene = Store::Get<Scene>(__current_key);
	if (!currentScene) return;

	Utility::Time::Start();
	Utility::Input::Update();
	
	currentScene->update();
	currentScene->render();

	if (__go_next) {
		const auto & nextScene = Store::Get<Scene>(__next_key);
		if (nextScene) {
			__go_next = false;
			nextScene->start();
			__finalizer = { __current_key, true };
			// �L�[�̐؂�ւ�
			__current_key = __next_key;
			Utility::Time::RegisterTimer("FinalizingScene", 1.0f, [&] {
				const auto & finalizeScene = Store::Get<Scene>(__finalizer.scene);
				finalizeScene->stop();
				finalizeScene->finalize();
				__finalizer.isFinaliing = false;
			});
		}
	}

	Utility::Time::Stop();
	Utility::Time::CheckTimers();
}

void System::onNext() {
	if (__current_key == __next_key) { // ���̃V�[���L�[���؂�ւ����Ă��Ȃ�
		Utility::PubSub<Event, void>::Publish(Event::End);
		return;
	}

	// ����������ĂȂ������珉�������܂��傤
	const auto & nextScene = Store::Get<Scene>(__next_key);
	if (!nextScene) return;

	if (nextScene->getState() != Scene::State::Initialized) {
		nextScene->initialize();
	}

	__go_next = true;
}

void System::onEnd() {
	// End���T�u�X�N���C�u���Ă���̂��������g�����Ȃ�
	if (Utility::PubSub<Event, void>::GetSubscriberCount(Event::End) == 1) {
		Utility::Window::Stop();
	}
}

void System::onDestroy() {
	__is_thread_running = false;
	if (__thread.joinable()) __thread.join();
	for (const auto & key : keys) {
		if (const auto & scene = Store::Get<Scene>(Store::Get<Scene>(key)->uuid)) {
			scene.get()->destroyIndex(key);
		}
	}
	Utility::Input::Finalize();
}

System::System(Key firstKey, std::unordered_map<Key, Scene*> scenes)
	: __current_key(firstKey), __next_key(firstKey), __is_thread_running(true) {
	Utility::PubSub<Event, void>::RegisterSubscriber(Event::Next, [&] { onNext(); });
	Utility::PubSub<Event, void>::RegisterSubscriber(Event::End, [&] { onEnd(); });
	
	keys.reserve(scenes.size());
	for (auto & [key, scene] : scenes) {
		bind<Scene>(scene).makeIndex(key);
		keys.emplace_back(key);
	}

	// �ŏ��̃V�[�������������ĊJ�n
	const auto & currentScene = Store::Get<Scene>(__current_key);
	currentScene->initialize();
	currentScene->start();

	Utility::Input::Initialize();
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

void System::Destroy() {
	delete __instance;
	__instance = nullptr;
}

void System::SetNext(Key key) {
	__instance->__next_key = key;
}

void System::LoadNext() {
	// �O�̃V�[���̏I�����ɂ��ȃo�J
	if (__instance->__finalizer.isFinaliing) return;

	__instance->__load_next_scene = std::thread([&] {
		Store::Get<Scene>(__instance->__next_key)->initialize();
	});
}

void System::LoadNextSync() {
	// �O�̃V�[���̏I�����ɂ��ȃo�J
	if (__instance->__finalizer.isFinaliing) return;

	Store::Get<Scene>(__instance->__next_key)->initialize();
}

bool System::IsNextSceneInitialized() {
	const auto result = Store::Get<Scene>(__instance->__next_key)->getState() == Scene::State::Initialized;
	if (result && __instance->__load_next_scene.joinable()) {
		__instance->__load_next_scene.join();
	}
	return result;
}
